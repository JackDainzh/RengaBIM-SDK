/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#include <optional>
#include <unordered_map>


using NodeId    = int;    ///< Local id
using RouteId   = int;    ///< Identifier of a route to whitch node belongs
using Parameter = double; ///< Position of the node on the route
using DeviceId  = int;    ///< Identifier of the node
using PortIndex = int;    ///< Port index

struct Node
{
  NodeId localId;
  std::unordered_map<RouteId, Parameter> intersections;
  std::optional<DeviceId> oDeviceId;
  std::optional<DeviceId> oObjectWithPortsId;
  std::optional<PortIndex> oPortIndex;
};
