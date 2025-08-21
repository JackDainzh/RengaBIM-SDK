/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#include "AlgorithmForSearchPaths.h"
#include "Utils.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <ranges>
#include <utility>


namespace
{
  class Wardrobe
  {
  public:
    int getId() const { return m_counter++; }

  private:
    mutable int m_counter = 0;
  };

  struct NodeComparator
  {
    const RouteId routeId;
    const Parameter parameter;

    bool operator()(const Node& node) const
    {
      return node.intersections.contains(routeId) && isEqual(node.intersections.at(routeId), parameter);
    }
  };

  std::unordered_map<RouteId, Renga::IRouteParamsPtr> extractRouteParamsFrom(
      const Renga::IModelObjectCollectionPtr objectCollection)
  {
    auto routes = std::unordered_map<RouteId, Renga::IRouteParamsPtr>{};

    for (auto i = 0; i < objectCollection->Count; ++i)
    {
      auto object = objectCollection->GetByIndex(i);

      auto pRouteParam = Renga::IRouteParamsPtr{};
      object->QueryInterface(&pRouteParam);
      if (!pRouteParam)
        continue;

      routes[object->GetId()] = pRouteParam;
    }

    return routes;
  }

  void addOrUpdateObjectsOnRoute(
      std::vector<Node>& nodes,
      RouteId routeId,
      Renga::IRouteParamsPtr pRouteParam,
      const Wardrobe& wardrobe)
  {
    const auto objectOnRouteCount = pRouteParam->GetObjectOnRouteCount();
    for (auto i = 0; i < objectOnRouteCount; ++i)
    {
      const auto pObjectOnRoutePlacement = pRouteParam->GetObjectOnRoutePlacement(i);

      // Check, maybe this node already exists
      const auto deviceId    = pObjectOnRoutePlacement->Id;
      const auto nodeExsists = [&](const auto& node) { return node.oDeviceId == deviceId; };

      auto it = std::ranges::find_if(nodes, nodeExsists);

      if (it != nodes.end() && !it->oObjectWithPortsId)
        it->intersections[routeId] = pObjectOnRoutePlacement->parameter;
      else
      {
        auto node                   = Node{};
        node.oDeviceId              = deviceId;
        node.intersections[routeId] = pObjectOnRoutePlacement->parameter;
        node.localId                = wardrobe.getId();
        nodes.push_back(node);
      }
    }
  }

  void addOrUpdateJoints(
      std::vector<Node>& nodes,
      RouteId routeId,
      Renga::IRouteParamsPtr pRouteParam,
      const Wardrobe& wardrobe,
      const std::unordered_map<RouteId, Renga::IRouteParamsPtr>& routes)
  {
    const auto jointCount = pRouteParam->GetJointCount();
    for (auto i = 0; i < jointCount; ++i)
    {
      const auto jointParam                     = pRouteParam->GetJointParams(i);
      const auto jointRouteId                   = jointParam.routeId;
      const auto& pJoinedRoute                  = routes.at(jointRouteId);
      const auto joinedRouteMinParameter        = pJoinedRoute->GetContour()->MinParameter;
      const auto joinedRouteMaxParameter        = pJoinedRoute->GetContour()->MaxParameter;
      const auto isJoinedRouteConnectedByItsEnd = pJoinedRoute->TargetModelObjectId == routeId;
      const auto connectionParameter =
          isJoinedRouteConnectedByItsEnd ? joinedRouteMaxParameter : joinedRouteMinParameter;

      // Check, maybe this node already exists
      auto it = std::ranges::find_if(nodes, NodeComparator{.routeId = routeId, .parameter = jointParam.parameter});

      if (it != nodes.end())
        it->intersections[jointRouteId] = connectionParameter;
      else
      {
        auto node                        = Node{};
        node.intersections[jointRouteId] = connectionParameter;
        node.intersections[routeId]      = jointParam.parameter;
        node.localId                     = wardrobe.getId();
        nodes.push_back(node);
      }
    }
  }

  void addOrUpdateObjectsWithPorts(
      std::vector<Node>& nodes,
      RouteId routeId,
      Renga::IRouteParamsPtr pRouteParam,
      const Wardrobe& wardrobe)
  {
    const auto pRouteContour = pRouteParam->GetContour();
    auto routeEnds           = std::map<std::pair<DeviceId, PortIndex>, Parameter>{};
    const auto source        = std::make_pair(pRouteParam->SourceModelObjectId, pRouteParam->SourcePortIndex);
    const auto target        = std::make_pair(pRouteParam->TargetModelObjectId, pRouteParam->TargetPortIndex);

    routeEnds[source] = pRouteContour->MinParameter;
    routeEnds[target] = pRouteContour->MaxParameter;

    for (const auto& [routeEnd, param] : routeEnds)
    {
      const auto& [endId, endIndex] = routeEnd;

      if (endIndex == -1)
        continue;

      // Check, maybe this node already exists
      auto it = std::ranges::find_if(nodes, NodeComparator{.routeId = routeId, .parameter = param});

      if (it != nodes.end())
      {
        it->oObjectWithPortsId = endId;
        it->oPortIndex         = endIndex;
      }
      else
      {
        auto node                   = Node{};
        node.oObjectWithPortsId     = endId;
        node.oPortIndex             = endIndex;
        node.intersections[routeId] = param;
        node.localId                = wardrobe.getId();
        nodes.push_back(node);
      }
    }
  }

  std::vector<Node> extractNodesFrom(const std::unordered_map<RouteId, Renga::IRouteParamsPtr>& routes)
  {
    auto nodes    = std::vector<Node>{};
    auto wardrobe = Wardrobe{};

    for (const auto& [routeId, pRouteParam] : routes)
    {
      addOrUpdateObjectsOnRoute(nodes, routeId, pRouteParam, wardrobe);
      addOrUpdateJoints(nodes, routeId, pRouteParam, wardrobe, routes);
      addOrUpdateObjectsWithPorts(nodes, routeId, pRouteParam, wardrobe);
    }

    return nodes;
  }

  std::vector<Node> findNeighboursOnSameRoutes(const Node& inputNode, const std::vector<Node>& nodes)
  {
    using namespace std::ranges;
    using namespace std::views;

    auto allNodesLiesOnTheSameRoutesAsMe = std::vector<Node>{};

    for (auto routeId : inputNode.intersections | keys)
    {
      const auto byLayOnSameRoute = [&](const auto& otherNode) { return otherNode.intersections.contains(routeId); };
      auto nodesOnTheSameRoute    = nodes | filter(byLayOnSameRoute) | to<std::vector>();

      const auto byParameter = [&](const auto& lhs, const auto& rhs)
      { return lhs.intersections.at(routeId) < rhs.intersections.at(routeId); };
      sort(nodesOnTheSameRoute, byParameter);

      const auto itIsMe = [&](const auto& otherNode) { return otherNode.localId == inputNode.localId; };
      const auto me     = find_if(nodesOnTheSameRoute, itIsMe);

      const auto fromBeginToMe = std::distance(nodesOnTheSameRoute.begin(), me);
      const auto fromMeToEnd   = std::distance(me, nodesOnTheSameRoute.end());

      if (fromBeginToMe != 0) // There is at least one node on the left
      {
        auto previousNode = me;
        std::advance(previousNode, -1);
        allNodesLiesOnTheSameRoutesAsMe.push_back(*previousNode);
      }

      if (fromMeToEnd > 1) // there is at least one node on the right
      {
        auto nextNode = me;
        std::advance(nextNode, 1);
        allNodesLiesOnTheSameRoutesAsMe.push_back(*nextNode);
      }
    }

    return allNodesLiesOnTheSameRoutesAsMe;
  }


  std::vector<Node> findNodesBelongsTo(DeviceId id, const std::vector<Node>& nodes)
  {
    using namespace std::ranges;
    using namespace std::views;

    const auto isObjectWithPort = [&](const auto& node) { return node.oObjectWithPortsId == id; };
    const auto isObjectOnRoute  = [&](const auto& node) { return node.oDeviceId == id; };
    const auto byMepDevice      = [&](const auto& node) { return isObjectWithPort(node) || isObjectOnRoute(node); };

    return nodes | filter(byMepDevice) | to<std::vector>();
  }

  std::vector<Node> findNeighboursInsideObjectWithPorts(const Node& inputNode, const std::vector<Node>& nodes)
  {
    using namespace std::ranges;
    using namespace std::views;

    const auto nodesInObjectWithPortsBesidesMe = [&](const auto& otherNode)
    { return otherNode.localId != inputNode.localId && otherNode.oObjectWithPortsId == inputNode.oObjectWithPortsId; };

    return nodes | filter(nodesInObjectWithPortsBesidesMe) | to<std::vector>();
  }

  std::vector<Node> findAllNeighbours(const Node& inputNode, const std::vector<Node>& nodes)
  {
    auto neighbours = std::vector<Node>{};

    neighbours.append_range(findNeighboursOnSameRoutes(inputNode, nodes));

    if (inputNode.oObjectWithPortsId.has_value())
      neighbours.append_range(findNeighboursInsideObjectWithPorts(inputNode, nodes));

    return neighbours;
  }

  std::vector<Node> findPathBetween(const Node& begin, DeviceId endId, const std::vector<Node>& nodes)
  {
    using namespace std::ranges;
    using namespace std::views;

    auto passQueue   = std::queue<Node>{};
    auto visitedList = std::vector<bool>(nodes.size(), false);
    auto parentsList = std::unordered_map<NodeId, Node>{};

    visitedList[begin.localId] = true;

    const auto setParent = [&](const auto& parent)
    { return [&](auto& neighbour) { parentsList[neighbour.localId] = parent; }; };

    // We assume begin node doesn't connect with other nodes inside its object with ports
    auto neighbours = findNeighboursOnSameRoutes(begin, nodes);

    for_each(neighbours, setParent(begin));
    passQueue.push_range(std::move(neighbours));

    auto finishNode         = std::optional<Node>{};
    const auto isNotVisited = [&](const auto& node) { return !visitedList[node.localId]; };
    const auto isNodeWithId = [](const auto& node, auto id)
    { return (node.oObjectWithPortsId == id) || (node.oDeviceId == id); };

    while (!passQueue.empty())
    {
      const auto& node = passQueue.front();

      if (isNodeWithId(node, endId))
      {
        finishNode = node;
        break;
      }

      visitedList[node.localId] = true;

      auto notVisitedNeighbours = findAllNeighbours(node, nodes) | filter(isNotVisited) | to<std::vector>();
      for_each(notVisitedNeighbours, setParent(node));
      passQueue.pop();
      passQueue.push_range(std::move(notVisitedNeighbours));
    }

    auto path = std::vector<Node>{};

    if (!finishNode)
      return path;

    path.push_back(finishNode.value());

    auto localId = finishNode->localId;
    while (parentsList.contains(localId))
    {
      const auto& pParent = parentsList.at(localId);
      localId             = pParent.localId;
      path.push_back(pParent);
    }

    std::ranges::reverse(path);
    return path;
  }

  std::vector<std::vector<Node>> findPathsBetween(
      DeviceId beginDeviceId,
      DeviceId finishDeviceId,
      const std::vector<Node>& nodes)
  {
    auto paths = std::vector<std::vector<Node>>{};

    const auto beginNodes = findNodesBelongsTo(beginDeviceId, nodes); // One device could belong to different nodes
    if (beginNodes.empty())
      return paths;

    if (findNodesBelongsTo(finishDeviceId, nodes).empty())
      return paths;

    for (const auto& beginNode : beginNodes)
      paths.emplace_back(findPathBetween(beginNode, finishDeviceId, nodes));

    return paths;
  }
}

std::vector<std::vector<Node>> findPathsBetween(
    DeviceId a,
    DeviceId b,
    const Renga::IModelObjectCollectionPtr pObjectCollection)
{
  const auto routeParams = extractRouteParamsFrom(pObjectCollection);
  const auto nodes       = extractNodesFrom(routeParams);

  return findPathsBetween(a, b, nodes);
}
