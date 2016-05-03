#include <iostream>
#include <set>
#include <array>

using node_id = int;

struct node_data
{
    // Preceding nodes.
    std::set<node_id> _prev;

    // Following nodes.
    std::set<node_id> _next;

    // Value.
    std::string _value;
};

// All nodes in the graph.
std::array<node_data, 100> datas;
std::set<node_id> nodes;

auto& data_of(node_id i)
{
    return datas[i];
}

auto& prev_of(node_id i)
{
    return data_of(i)._prev;
}

auto& next_of(node_id i)
{
    return data_of(i)._next;
}

auto& value_of(node_id i)
{
    return data_of(i)._value;
}

auto& first_next(node_id i)
{
    return *(std::begin(next_of(i)));
}

void make_node(node_id i, std::string value)
{
    prev_of(i).clear();
    next_of(i).clear();
    data_of(i)._value = value;
    nodes.emplace(i);
}

void add_prev_to(node_id a, node_id b)
{
    prev_of(a).emplace(b);
    next_of(b).emplace(a);
}

// Merges `b` into `a`, then deletes `b`.
void merge(node_id a, node_id b)
{
    for(auto bp : prev_of(b))
    {
        prev_of(a).emplace(bp);
    }

    for(auto bn : next_of(b))
    {
        next_of(a).emplace(bn);
    }

    for(auto bn : next_of(b))
    {
        prev_of(bn).erase(b);
        prev_of(bn).emplace(a);
    }

    for(auto bp : prev_of(b))
    {
        next_of(bp).erase(b);
        next_of(bp).emplace(a);
    }

    prev_of(a).erase(a);
    next_of(a).erase(a);

    nodes.erase(b);
}

// Tries to apply merging tranformations, returns `true` if it succeeds.
bool process(node_id x)
{
    // `(*) -> (x) -> (then) -> (*)`
    if(next_of(x).size() == 1 && prev_of(first_next(x)).size() == 1)
    {
        auto then = first_next(x);
        value_of(x) += ".then(" + value_of(then);
        merge(x, then);
        value_of(x) += ")";

        return true;
    }

    // Deal with fork/join configurations.
    for(auto n : next_of(x))
    {
        if(prev_of(n).size() == 1 && next_of(n).size() == 1)
        {
            auto target = first_next(n);

            std::set<node_id> parallel;
            for(auto other : next_of(x))
            {
                if(other != n && prev_of(other).size() == 1 &&
                    next_of(other).size() == 1 && first_next(other) == target)
                {
                    parallel.emplace(other);
                }
            }

            if(parallel.size() > 0)
            {
                std::string sb = "when_all(";
                sb += value_of(n);

                for(auto other : parallel)
                {
                    sb += ", " + value_of(other);
                    merge(n, other);
                }

                sb += ")";
                value_of(n) = sb;
                return true;
            }
        }
    }

    return false;
}

/*
                /-> B0 -\
        /-> B -<         >-\
        |       \-> B1 -/   >-> D -\
    A -<                   /        \
     \  |                 /          \
      \ \-> C -----------/            >-> F
       \                             /
        \                           /
         \--------------------> E -/
*/

/*
A.then
(
    when_all
    (
        when_all
        (
            B.then
            (
                when_all(B0, B1)
            ),
            C
        ).then(D),
        E
    )
).then(F)
*/



int main()
{
    make_node(0, "A");
    make_node(1, "B");
    make_node(6, "B0");
    make_node(7, "B1");
    make_node(2, "C");
    make_node(3, "D");
    make_node(4, "E");
    make_node(5, "F");

    // D -> F
    add_prev_to(5, 3);

    // E -> F
    add_prev_to(5, 4);

    // A -> E
    add_prev_to(4, 0);

    // B0 -> D
    add_prev_to(3, 6);

    // B1 -> D
    add_prev_to(3, 7);

    // C -> D
    add_prev_to(3, 2);

    // A -> B
    add_prev_to(1, 0);

    // B -> B0
    add_prev_to(6, 1);

    // B -> B1
    add_prev_to(7, 1);

    // A -> C
    add_prev_to(2, 0);

    bool any_change;

    do
    {
        any_change = false;

        for(auto n : nodes)
        {
            if(process(n))
            {
                any_change = true;
                break;
            }
        }
    } while(any_change);

    for(auto x : nodes)
    {
        std::cout << value_of(x);
    }

    std::cout << std::endl;
}