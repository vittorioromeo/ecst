#!/bin/python3 

import sys
import re
import time

src = sys.stdin.read()
out = ""
angle_pairs = []
marked = []
desired_depth = 0 if len(sys.argv) <= 1 else int(sys.argv[1])
namespace_replacements = []
other_replacements = []

def ns_replacement(xfrom, xto):
    namespace_replacements.append((xfrom, xto))
    
def replacement(xfrom, xto):
    other_replacements.append((xfrom, xto))

# Find all `<...>` pair ranges
def find_angles(s, xi_start, xi_end):
    open_angles = []

    for i in range(xi_start, xi_end):
        if s[i-1] != ' ' and s[i] == '<':
            open_angles.append(i)

        elif s[i-1] != '-' and s[i] == '>':
            assert(len(open_angles) > 0)

            depth = len(open_angles)
            x = open_angles.pop()
            angle_pairs.append((x, i+1, depth))

# Mark a `<...>` pair for removal
def mark(i_start, i_end):
    marked.append((i_start + 1, i_end - 1))

# Yield merged overlapping intervals
# (From 'http://codereview.stackexchange.com/questions/69242/merging-overlapping-intervals')
def merged(intervals):
    sorted_intervals = sorted(intervals, key=lambda x: x[0])

    if not sorted_intervals:  # no intervals to merge
        return

    # low and high represent the bounds of the current run of merges
    low, high = sorted_intervals[0]

    for iv in sorted_intervals[1:]:
        if iv[0] <= high:  # new interval overlaps current run
            high = max(high, iv[1])  # merge with the current run
        else:  # current run is over
            yield low, high  # yield accumulated interval
            low, high = iv  # start new run

    yield low, high  # end the final run

# Namespace replacements
ns_replacement('boost::hana', 'bh')
ns_replacement('vrm::core', 'vrmc')
ns_replacement('std', '')
ns_replacement('ecst', '')
ns_replacement('impl', '_')
ns_replacement('system_execution_adapter', 'sea')
ns_replacement('context', 'ctx')
ns_replacement('tag::system', 'tag_s')
ns_replacement('tag::component', 'tag_c')

# Non-namespace replacements
replacement(r'integral_constant', r'ic')
replacement(r'lambda\(', r'[](')
replacement(r'size_t', r'sz_t')
replacement(r'unsigned ', r'u')
replacement(r'long u', r'ul')
replacement(r'type_impl', r'type')
replacement(r'tag_impl', r'tag')
replacement(r'<>', r'<?>')

# Find all angle bracket pairs
find_angles(src, 0, len(src))

# Mark pairs matching desired depth for removal
for p in angle_pairs:
    if p[2] > desired_depth:
        mark(p[0], p[1])

# Build output string by avoiding marked intervals
last = 0
for m in merged(marked):
    out += src[last:m[0]]
    last = m[1]
out += src[last:len(src)]

# Replace namespaces 
for nr in namespace_replacements:
    out = re.sub(nr[0] + r'::', nr[1] + r'::' if len(nr[1]) > 0 else '', out)

# Replace everything else
for r in other_replacements:
    out = re.sub(r[0], r[1], out)

# Print result
print(out)
