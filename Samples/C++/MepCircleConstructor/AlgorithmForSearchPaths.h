/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>

#include "Node.h"

#include <vector>

std::vector<std::vector<Node>> findPathsBetween(DeviceId, DeviceId, const Renga::IModelObjectCollectionPtr);
