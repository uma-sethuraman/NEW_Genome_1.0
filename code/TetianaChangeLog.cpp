
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <memory>


void delete_mutation(int ind, std::vector<int>& genome, std::vector<int>& mutations) {
    
    int before_inds_num = count_if (mutations.begin(), mutations.end(), [ind](int i){ return i < ind; });
    mutations.push_back(ind - before_inds_num);
    std::cout << "pushing: " << ind - before_inds_num << std::endl;
    
}

void reconstruct(std::vector<int>& genome, std::vector<int>& mutations) {
    for (auto it = mutations.begin(); it != mutations.end(); ++it) {
        genome.erase(genome.begin() + (*it)); // (*it) it index to erase
    }
}


int main()
{
    std::vector<int> genome{0, 1, 2, 3, 4, 5, 6};
    
    //std::set<std::vector<int>::iterator> mutations;
    //std::unique_ptr<int> first_deletion = nullptr;
    //std::vector<int>::iterator first_deletion = genome.end();
    std::vector<int> mutations;
    
    //==============
    auto print = [](const int& n) { std::cout << " " << n; };
 
    std::cout << "genome before mutations:\n";
    std::for_each(genome.cbegin(), genome.cend(), print);
    std::cout << '\n';
    
    delete_mutation(2, genome, mutations);
    delete_mutation(3, genome, mutations);
    delete_mutation(6, genome, mutations);
    delete_mutation(4, genome, mutations);
    
    //if (first_deletion != genome.end())
    //    std::cout << "first_deletion: " << *first_deletion << std::endl;
    
    std::cout << "genome after mutations:\n";
    std::for_each(genome.cbegin(), genome.cend(), print);
    std::cout << '\n';
    
    reconstruct(genome, mutations);
    
    std::cout << "genome after reconstruction:\n";
    std::for_each(genome.cbegin(), genome.cend(), print);
    std::cout << '\n';

    return 0;
}

