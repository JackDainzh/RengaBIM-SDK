/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#include "AlgorithmForSearchPaths.h"
#include "CircleBuilder.h"
#include "Utils.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <ranges>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include <atlsafe.h>


CircleBuilder::CircleBuilder(Renga::IActionPtr pAction, const Renga::IApplicationPtr pApp)
  : Renga::ActionEventHandler(pAction),
    m_pApp(pApp)
{
}

void CircleBuilder::OnTriggered()
{
  const auto selected = CComSafeArray<int>{m_pApp->Selection->GetSelectedObjects()};
  if (selected.GetCount() != 2)
    return;

  const auto pObjectCollection = m_pApp->Project->Model->GetObjects();

  const auto objectId1 = selected.GetAt(0);
  const auto objectId2 = selected.GetAt(1);

  const auto paths = findPathsBetween(objectId1, objectId2, pObjectCollection);

  auto ss = std::wstringstream{};

  for (const auto& path : paths)
  {
    if (path.empty())
      continue;

    const auto size = path.size();
    assert((void("There are only one elements in the path"), size > 1));

    for (auto i = 0; i < size - 1; ++i)
    {
      const auto& node1 = path.at(i);
      const auto& node2 = path.at(i + 1);

      const auto segment = getRouteSegmentBetween(node1, node2);

      ss << L"Node: " << nodeInfo(node1) << ", ";
      if (segment)
        ss << L"Segment: " << segmentInfo(segment.value()) << ", ";
    }
    ss << L"Node: " << nodeInfo(path.back()) << ";\n\n";
  }

  auto message = ss.str();

  if (message.empty())
    message = std::format(L"Oops, it seems there is no path between {} and {}", objectId1, objectId2);

  m_pApp->UI->ShowMessageBox(Renga::MessageIcon::MessageIcon_Info, L"MepCirclesConstructorPlugin", message.c_str());
}

std::wstring CircleBuilder::nodeInfo(const Node& node) const
{
  auto info = std::wstring{L"{"};

  if (node.oObjectWithPortsId && node.oPortIndex)
    info.append(std::format(L"(owp id = {} port = {}) ", node.oObjectWithPortsId.value(), node.oPortIndex.value()));

  if (node.oDeviceId)
    info.append(std::format(L"(device id = {}) ", node.oDeviceId.value()));

  if (info == L"{")
    for (auto routeId : node.intersections | std::views::keys)
      info.append(std::format(L"(route id = {} parameter = {}) ", routeId, node.intersections.at(routeId)));

  const auto lastSpace  = info.find_last_of(L" ");
  const auto resultInfo = info.substr(0, lastSpace).append(L"}");

  return resultInfo;
}

std::wstring CircleBuilder::segmentInfo(const CircleBuilder::Segment& segment) const
{
  auto info = std::wstring{L"{"};

  if (const auto deviceId = std::get_if<DeviceId>(&segment))
    info.append(std::format(L"flow segment id = {}", *deviceId));
  else if (const auto routePart = std::get_if<RoutePart>(&segment))
    info.append(
        std::format(L"route id = {} begin = {} end = {}", routePart->routeId, routePart->begin, routePart->end));

  info.append(L"}");

  return info;
}

std::optional<CircleBuilder::Segment> CircleBuilder::getRouteSegmentBetween(const Node& a, const Node& b) const
{
  const auto routesFromAObject = a.intersections | std::views::keys | std::ranges::to<std::set>();
  const auto routesFromBObject = b.intersections | std::views::keys | std::ranges::to<std::set>();

  auto intersections = std::vector<RouteId>{};

  std::ranges::set_intersection(routesFromAObject, routesFromBObject, std::back_inserter(intersections));
  if (intersections.empty())
    return std::nullopt;

  const auto connectedRouteId = intersections.at(0);

  const auto parameterA = a.intersections.at(connectedRouteId);
  const auto parameterB = b.intersections.at(connectedRouteId);

  const auto beginParam = std::min<decltype(parameterA)>(parameterA, parameterB);
  const auto endParam   = std::max<decltype(parameterA)>(parameterA, parameterB);

  const auto pObject = m_pApp->Project->Model->GetObjects()->GetById(connectedRouteId);
  auto pMepParam     = Renga::IRouteParamsPtr{};

  pObject->QueryInterface(&pMepParam);

  const auto flowSegmentCount = pMepParam->GetFlowSegmentCount();
  for (auto i = 0; i < flowSegmentCount; ++i)
  {
    const auto pFlowSegment = pMepParam->GetFlowSegmentPlacement(i);
    const auto pBaseLine    = pFlowSegment->GetBaseline();

    auto beginPoint = pBaseLine->GetBeginPoint();
    auto endPoint   = pBaseLine->GetEndPoint();

    const auto projectionBeginPointOnRoute = pMepParam->GetContour()->PointProjection(&beginPoint);
    const auto projectionEndPointOnRoute   = pMepParam->GetContour()->PointProjection(&endPoint);

    if (lessOrEqual(beginParam, projectionBeginPointOnRoute) && lessOrEqual(projectionEndPointOnRoute, endParam))
      return pFlowSegment->Id;
  }

  return RoutePart{.routeId = connectedRouteId, .begin = beginParam, .end = endParam};
}
