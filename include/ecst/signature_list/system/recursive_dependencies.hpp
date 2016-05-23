// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/system/id.hpp>
#include <ecst/signature_list/system/dependents.hpp>

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    // TODO: refactor/beautify
    namespace impl
    {
        template <typename TSystemSignatureList, typename TSystemSignature>
        auto recursive_dependency_id_list_impl(
            TSystemSignatureList ssl, TSystemSignature ss)
        {
            // Initial dependencies of `ss` list.
            auto dependencies_list = // .
                signature_list::system::dependencies_ids_list(ssl, ss);

            // Recursive step implementation.
            auto step = [=](auto self, auto curr_list)
            {
                return mp::list::unique_cat( // .
                    curr_list,               // .
                    mp::bh::fold_left(curr_list, mp::list::empty_v,
                        [=](auto acc, auto xid)
                        {
                            auto xsig = signature_by_id(ssl, xid);
                            auto new_list = dependencies_ids_list(ssl, xsig);

                            return mp::list::unique_cat(acc, self(new_list));
                        }));
            };

            // Start the recursion.
            return vrmc::y_combinator(step)(dependencies_list);
        }
    }

    /// @brief Returns a list of the IDs of all dependencies (recursively) of
    /// `ss`.
    template <typename TSystemSignatureList, typename TSystemSignature>
    constexpr auto recursive_dependency_id_list(
        TSystemSignatureList ssl, TSystemSignature ss)
    {
        return decltype(impl::recursive_dependency_id_list_impl(ssl, ss)){};
    }

    namespace impl
    {
        template <typename TSystemSignatureList, typename TSystemSignature,
            typename TDependencySignature>
        auto has_dependency_recursive_impl(TSystemSignatureList ssl,
            TSystemSignature ss, TDependencySignature ds)
        {
            auto ds_id = id_by_signature(ssl, ds);

            return mp::bh::contains(
                recursive_dependency_id_list(ssl, ss), ds_id);
        }
    }

    /// @brief Returns `true` if `ds` is in the recursive dependency list of
    /// `ss`.
    template <typename TSystemSignatureList, typename TSystemSignature,
        typename TDependencySignature>
    constexpr auto has_dependency_recursive(
        TSystemSignatureList ssl, TSystemSignature ss, TDependencySignature ds)
    {
        return decltype(impl::has_dependency_recursive_impl(ssl, ss, ds)){};
    }


    namespace impl
    {
        namespace bf_traversal_context
        {
            using namespace mp;

            namespace impl
            {
                template <typename TStartNode>
                auto make_impl(TStartNode sn)
                {
                    return bh::make_pair(
                        bh::make_basic_tuple(sn), bh::make_basic_tuple());
                }
            }

            template <typename TStartNode>
            constexpr auto make(TStartNode sn)
            {
                return decltype(impl::make_impl(sn)){};
            }

            template <typename TBFTContext>
            constexpr auto queue(TBFTContext c)
            {
                return bh::first(c);
            }

            template <typename TBFTContext>
            constexpr auto visited(TBFTContext c)
            {
                return bh::second(c);
            }

            template <typename TBFTContext, typename TNode>
            constexpr auto is_visited(TBFTContext c, TNode n)
            {
                return bh::contains(visited(c), n);
            }

            template <typename TBFTContext, typename TNode>
            constexpr auto is_in_queue(TBFTContext c, TNode n)
            {
                return bh::contains(queue(c), n);
            }

            template <typename TBFTContext>
            constexpr auto is_queue_empty(TBFTContext c)
            {
                return bh::is_empty(queue(c));
            }

            template <typename TBFTContext>
            constexpr auto top_node(TBFTContext c)
            {
                return bh::at(queue(c), mp::sz_v<0>);
            }

            namespace impl
            {
                template <typename TBFTContext, typename TSSL>
                auto step_forward_impl(TBFTContext c, TSSL ssl)
                {
                    auto node = top_node(c);
                    auto popped_queue = bh::remove_at(queue(c), mp::sz_v<0>);
                    auto neighbors =
                        dependent_ids_list(ssl, signature_by_id(ssl, node));

                    auto unvisited_neighbors =
                        bh::remove_if(neighbors, [=](auto x_nbr)
                            {
                                return is_visited(c, x_nbr);
                            });

                    auto new_visited =
                        list::unique_cat(visited(c), unvisited_neighbors);

                    auto new_queue =
                        list::unique_cat(popped_queue, unvisited_neighbors);

                    return bh::make_pair(new_queue, new_visited);
                }
            }

            template <typename TBFTContext, typename TSSL>
            constexpr auto step_forward(TBFTContext c, TSSL ssl)
            {
                return decltype(impl::step_forward_impl(c, ssl)){};
            }
        }

        namespace impl
        {
            template <typename TStartNode, typename TSSL>
            auto bf_traversal_impl(TStartNode sn, TSSL ssl)
            {
                using namespace mp;
                namespace btfc = bf_traversal_context;

                auto step = [=](auto self, auto ctx)
                {
                    return static_if(btfc::is_queue_empty(ctx))
                        .then([=](auto)
                            {
                                return bh::make_basic_tuple();
                            })
                        .else_([=](auto x_ctx)
                            {
                                auto node = btfc::top_node(x_ctx);

                                return list::unique_cat(
                                    bh::make_basic_tuple(node),
                                    self(btfc::step_forward(x_ctx, ssl)));
                            })(ctx);
                };

                return vrmc::y_combinator(step)(btfc::make(sn));
            }
        }


        template <typename TSystemSignatureList, typename TSystemSignature>
        auto recursive_dependents_id_list_impl(
            TSystemSignatureList ssl, TSystemSignature parent)
        {
            using namespace mp;
            return impl::bf_traversal_impl(id_by_signature(ssl, parent), ssl);
        }
    }

    /// @brief Returns the set of dependent IDs of `parent`.
    template <typename TSystemSignatureList, typename TSystemSignature>
    constexpr auto recursive_dependents_id_list(
        TSystemSignatureList ssl, TSystemSignature parent)
    {
        return decltype(impl::recursive_dependents_id_list_impl(ssl, parent)){};
    }


    /// @brief Returns the number of systems that depend on `st`.
    template <typename TSystemSignatureList, typename TSystemTag>
    constexpr auto chain_size(TSystemSignatureList ssl, TSystemTag st)
    {
        return sz_v<mp::bh::size(recursive_dependents_id_list(
                   ssl, signature_by_tag(ssl, st)))> -
               sz_v<0>;
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
