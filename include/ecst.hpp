// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// Library API configuration, namespace macros, internal code.
#include <ecst/config.hpp>

// Type/function aliases.
#include <ecst/aliases.hpp>

// Utility module, with functions/types used throughout the project.
#include <ecst/utils.hpp>

// Debugging/logging utilities.
#include <ecst/debug.hpp>

// "Type-value encoding" metaprogramming.
#include <ecst/mp.hpp>

// Machine hardware status detection and querying.
#include <ecst/hardware.hpp>

// Inner parallelism strategies.
#include <ecst/inner_parallelism.hpp>

// Component and system signatures.
#include <ecst/tag.hpp>

// Component and system signatures.
#include <ecst/signature.hpp>

// Component and system signature lists.
#include <ecst/signature_list.hpp>

// System execution function adapters.
#include <ecst/system_execution_adapter.hpp>

// Thread pool utility class.
#include <ecst/thread_pool.hpp>

// Framework user-defined template settings.
#include <ecst/settings.hpp>

// Context/manager class.
#include <ecst/context.hpp>

// Implementation of make functions (default settings).
#include <ecst/make_inl.hpp>
