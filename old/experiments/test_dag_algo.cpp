#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <map>
#include <vector>
#include <string>

/*
             /-> B -\
begin -> A -<       >-> D -> E ->end
          \  \-> C -/      /
           \              /
            --------> F -/

depenendencies  |  dependents
----------------|-------------
[E]             :  [end]
[D, F]          :  [E]
[B, C]          :  [D]
[A]             :  [B, C, F]
[begin]         :  [A]
*/

using node = char;
using node_list = std::vector<char>;
using dependency_map = std::map<node_list, node_list>;

constexpr char n_end = '0';
constexpr char n_begin = '1';

dependency_map data // .
    {
        // {{dependencies}, {dependents}}
        {{'E'}, {n_end}},         // .
        {{'D', 'F'}, {'E'}},      // .
        {{'B', 'C'}, {'D'}},      // .
        {{'A'}, {'B', 'C', 'F'}}, // .
        {{n_begin}, {'A'}},       // .
    };

auto contains(const node_list& nl, node n)
{
    for(const auto& c : nl)
    {
        if(c == n)
        {
            return true;
        }
    }

    return false;
}

const node_list& dependencies(const dependency_map& m, node n)
{
    for(const auto& kv : m)
    {
        if(contains(kv.second, n))
        {
            return kv.first;
        }
    }

    throw;
}

const node_list& dependents(const dependency_map& m, node n)
{
    for(const auto& kv : m)
    {
        if(contains(kv.first, n))
        {
            return kv.second;
        }
    }

    throw;
}

auto is_begin(const dependency_map&, node n)
{
    return n == n_begin;
}

auto is_linear(const dependency_map& dm, node n)
{
    return dependencies(dm, n).size() == 1;
}

auto is_join(const dependency_map& dm, node n)
{
    return dependencies(dm, n).size() > 1;
}

/*
expand x = x                                         | is_begin(x)
           expand(dependency(x)).then(x)             | is_linear(x)
           merge(expand(dependencies(x))..., to(x))  | is_join(x)
*/

struct step
{
    std::string _name;
    node_list _nodes;

    step(const std::string& name, const node_list& nodes)
        : _name(name), _nodes(nodes)
    {
    }
};

void terminal(node n)
{
    std::cout << "terminal: " << n << "\n";
}

void then(node n)
{
    std::cout << "then: " << n << "\n";
}

// merge xs..., to(y) = (common_prefix([],
// xs...)).then(when_all(suffixes(xs)...).then(y))

auto merge(const dependency_map& dm, const node_list& ns, node n)
{
    // auto cp = common_prefix(node_list{}, ns)

    std::cout << "merging: ";
    for(const auto& n : ns) std::cout << n << ", ";
    std::cout << "\n";

    return 'j';
}

auto expand(const dependency_map& dm, node n)
{
    if(is_begin(dm, n))
    {
        std::cout << "(" << n << " is begin)"
                  << "\n";

        std::cout << n << "\n";
        return n;
    }

    if(is_linear(dm, n))
    {
        std::cout << "(" << n << " is linear)"
                  << "\n";

        auto res = expand(dm, dependencies(dm, n)[0]);
        std::cout << ".then(" << n << ")\n";
        return res;
    }

    if(is_join(dm, n))
    {
        std::cout << "(" << n << " is join)"
                  << "\n";

        auto deps = dependencies(dm, n);
        node_list expanded_deps;
        for(auto d : deps)
        {
            expanded_deps.emplace_back(expand(dm, d));
        }

        return merge(dm, expanded_deps, n);
    }

    throw;
}

int main()
{
    expand(data, n_end);
    return 0;
}