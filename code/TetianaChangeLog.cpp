
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <memory>

void delete_mutation(int ind, std::vector<int>& genome, std::set<int>& mutations, 
    std::vector<int>::iterator& first_deletion) {
    
    // TODO check if index is outside of boundaries
    
    if (first_deletion == genome.end()) { // first deletion
        first_deletion = genome.begin() + ind; //std::vector<int>::iterator to_del_it = genome.begin() + ind;
    } else {
        if (ind > (first_deletion - genome.begin())) { // index to delete is after first_deletion
            if (mutations.find(ind) != mutations.end()) {
                // already exists - need to delete next one
                mutations.insert(ind + 1); // what is ind + 1 also in the set
            } else {
                // update
                mutations.insert(ind);
            }
                
            
        } else if (ind < (first_deletion - genome.begin())) {
            std::vector<int>::iterator to_del_it = genome.begin() + ind;
            mutations.insert(*first_deletion);
            first_deletion = to_del_it;
        }  else {
            // if ind == (first_deletion - genome.begin()) - need to delete next one
            if (mutations.find(ind + 1) != mutations.end()) {
                // already exists - need to delete next one
                mutations.insert(ind + 2);
            } else {
                // update
                mutations.insert(ind + 1);
            }
        }
        
    }
    
}

void reconstruct(std::vector<int>& genome, std::set<int>& mutations, 
    std::vector<int>::iterator first_deletion) {
    
    if (first_deletion != genome.end()) {
        for (auto it = mutations.rbegin(); it != mutations.rend(); ++it) {
            genome.erase(/*first_deletion*/ genome.begin() + (*it)); // iterator + int
        }
        genome.erase(first_deletion); // iterator
    }
    
    
}


int main()
{
    std::vector<int> genome{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
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

