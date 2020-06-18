
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <memory>

void delete_mutation(int ind, std::vector<int>& genome, std::set<int>& mutations, 
    std::vector<int>::iterator& first_deletion) {
    
    // TODO check if index is outside of boundaries
    
    if (first_deletion == genome.end()) {
        // first deletion
        //std::cout << "first deletion" << std::endl;
        std::vector<int>::iterator to_del_it = genome.begin() + ind;
        //if (to_del_it != genome.end()) std::cout << "to_del_it: " << *to_del_it << std::endl;
        first_deletion = to_del_it;
    } else {
        if (genome[ind] > *first_deletion) {
            mutations.insert(ind); // TODO what if already exists - need to delete next one?
        } else if (genome[ind] < *first_deletion) {
            std::vector<int>::iterator to_del_it = genome.begin() + ind;
            mutations.insert(*first_deletion);
            first_deletion = to_del_it;
        } // TODO if genome[ind] == *first_deletion - need to delete next one?
    }
    
}

void reconstruct(std::vector<int>& genome, std::set<int>& mutations, 
    std::vector<int>::iterator first_deletion) {
    
    if (first_deletion != genome.end()) {
        for (auto it = mutations.rbegin(); it != mutations.rend(); ++it) {
            genome.erase(first_deletion + (*it)); // iterator + int
        }
        genome.erase(first_deletion); // iterator
    }
    
    
}


int main()
{
    std::vector<int> genome{0, 1, 2, 3, 4, 5};
    
    //std::set<std::vector<int>::iterator> mutations;
    //std::unique_ptr<int> first_deletion = nullptr;
    std::vector<int>::iterator first_deletion = genome.end();
    std::set<int> mutations;
    
    //==============
    auto print = [](const int& n) { std::cout << " " << n; };
 
    std::cout << "genome before mutations:\n";
    std::for_each(genome.cbegin(), genome.cend(), print);
    std::cout << '\n';
    
    delete_mutation(1, genome, mutations, first_deletion);
    delete_mutation(3, genome, mutations, first_deletion);
    delete_mutation(2, genome, mutations, first_deletion);
    
    //if (first_deletion != genome.end())
    //    std::cout << "first_deletion: " << *first_deletion << std::endl;
    
    std::cout << "genome after mutations:\n";
    std::for_each(genome.cbegin(), genome.cend(), print);
    std::cout << '\n';
    
    reconstruct(genome, mutations, first_deletion);
    
    std::cout << "genome after reconstruction:\n";
    std::for_each(genome.cbegin(), genome.cend(), print);
    std::cout << '\n';

    return 0;
}

