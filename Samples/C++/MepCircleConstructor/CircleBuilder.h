/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include <Renga/ActionEventHandler.hpp>

#include "Node.h"

#include <optional>
#include <string>
#include <variant>


namespace
{
  struct RoutePart
  {
    RouteId routeId;
    double begin;
    double end;
  };
}

class CircleBuilder : public Renga::ActionEventHandler
{
  using Segment = std::variant<DeviceId, RoutePart>;

public:
  CircleBuilder(Renga::IActionPtr, const Renga::IApplicationPtr);

  void OnTriggered() override;

  void OnToggled(bool) override {}

private:
  std::wstring nodeInfo(const Node&) const;
  std::wstring segmentInfo(const Segment& segment) const;

  std::optional<Segment> getRouteSegmentBetween(const Node&, const Node&) const;

private:
  const Renga::IApplicationPtr m_pApp;
};