# Keep the function signature,
# but replace its body with your implementation.
#
# Note that this is the driver function.
# Please write a well-structured implemention by creating other functions outside of this one,
# each of which has a designated purpose.
#
# As a good programming practice,
# please do not use any script-level variables that are modifiable.
# This is because those variables live on forever once the script is imported,
# and the changes to them will persist across different invocations of the imported functions.

#!/usr/bin/python

import os, sys, zlib

def topo_order_commits():
    path = git_dir()
    branches= get_branches(path)
    commit_info, root_commits, branch_hashes = build_commit_graph(path, branches)
    topo_order = get_topo_ordered_commits(commit_info, root_commits)
    #print_topo_ordered_commits_with_branch_names(commit_info, topo_order, branch_hashes)

    #print order 
    for i in range(len(topo_order)-1):
        curr_hash = topo_order[i]
        print(curr_hash, end = "")
        if curr_hash in branch_hashes:
            print(" " + branch_hashes[curr_hash])
        else:
            print("")

        #second part if next hash is not in parents  
        if i+1<len(topo_order) and topo_order[i+1] not in commit_info[curr_hash].parents:
            output = ""
            for hash in commit_info[curr_hash].parents:
                output += hash 
                output += " "
            print(output.rstrip(), end = "=\n\n")
            print("=", end = "")
            output = ""
            for hash in commit_info[topo_order[i+1]].children:
                output += hash 
                output += " "
            print(output.rstrip())

    #make sure to print last node
    print(topo_order[len(topo_order)-1], end = "")
    if topo_order[len(topo_order)-1] in branch_hashes:
        print(" " + branch_hashes[topo_order[len(topo_order)-1]])
    else:
        print("")


if __name__ == '__main__':
    topo_order_commits()


class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()


def git_dir():
    path = os.getcwd()
    for d in os.listdir(path):
        cpath = os.path.join(path, d)
        if (d==".git"):
            return (os.path.abspath(cpath))
        if path=='/':
            break
    path = os.path.dirname(path)
    sys.exit ("Not inside a git repository")


def get_branches(git_dir):
    #branch names are in the refs/head folder
    head = os.path.join(git_dir, "refs/heads")
    branches = []
    for root, dirs, files in os.walk(head):
        for f in files:
            if not os.path.isdir(f):
                path = os.path.join(root,f)
                branches.append(os.path.relpath(path, head))
    return branches


def build_commit_graph(git_dir, branches):

    commit_nodes = {}       
    branch_hash_names = {}       
    stack = []
    root_hashes = []

    #getting hashes for the branch (needed to print output)
    for branch in branches:
        branch_path = os.path.join(git_dir, "refs/heads", branch)
        with open(branch_path, "r") as file:
            branch_hash = file.read()[:-1]
            stack.append(branch_hash) 
            if branch_hash not in commit_nodes:
                node = CommitNode(branch_hash)
                commit_nodes[branch_hash] = node
            if branch_hash not in branch_hash_names:
                branch_hash_names[branch_hash] = branch
            else:
                branch_hash_names[branch_hash] = branch_hash_names[branch_hash]
    
    while stack:
        commit_hash = stack.pop()
        curr_node = commit_nodes[commit_hash]

        #find parents - first two chars in hash are folder name
        curr_path = os.path.join(git_dir, "objects", commit_hash[:2], commit_hash[2:])
        #decompressing object files to get parent commits
        data = zlib.decompress(open(curr_path, 'rb').read())
        data = data.decode()
        parent = ""
        #parent commits should be preceded by parents
        for line in data.split('\n'):
            if line.startswith("parent "):
                hash_parent = line[7:]
                if parent in commit_nodes:
                    parent_node = commit_nodes[hash_parent]
                else:
                    parent_node = CommitNode (hash_parent)
                    curr_node.parents.add(hash_parent)
                    parent_node.children.add(commit_hash)
                    commit_nodes[hash_parent]= parent_node
                    stack += [hash_parent]
            else:
                if commit_hash not in root_hashes:
                    root_hashes.append(commit_hash)

        commit_nodes[commit_hash] = curr_node
    return commit_nodes, root_hashes, branch_hash_names


def get_topo_ordered_commits(commit_nodes, root_hashes):
    #following pseudocode from slides...
    order = []
    visited = set()
    temp_stack = []
    stack = sorted(root_hashes)
    while stack:
        v = stack.pop()
        if v in visited:
            continue
        visited.add(v)
        while len(temp_stack) and v not in commit_nodes[temp_stack[-1]].children:
            g = temp_stack.pop()
            order.append(g)
        temp_stack.append(v)

        for c in sorted(commit_nodes[v].children):
            if c not in visited:
                stack.append(c)
    #add rest of temp_stack to order
    while len(temp_stack):
        order.append(temp_stack.pop())

    return order


#print function from TA hw hint slides, but did not work for me

# def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches): jumped = False
# for i in range(len(topo_ordered_commits)):
# commit_hash = topo_ordered_commits[i] if jumped:
# jumped = False
# sticky_hash = ' '.join(commit_nodes[commit_hash].children) print(f'={sticky_hash}')
# branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else [] print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
# if i+1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents: jumped = True
# sticky_hash = ' '.join(commit_nodes[commit_hash].parents) print(f'{sticky_hash}=\n')