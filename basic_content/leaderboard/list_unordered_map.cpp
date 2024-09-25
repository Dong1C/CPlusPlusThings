#include <iostream>
#include <unordered_map>
#include <list>

struct rankitem
{
    unsigned long charid; // 用户ID
    unsigned int score;   // 分数
    unsigned int rank;    // 排名
};

std::unordered_map<unsigned long, std::list<rankitem>::iterator> userMap;
std::list<rankitem> rankList;

void updateLeaderboard(unsigned long charid, unsigned int new_score)
{
    auto it_user = userMap.find(charid);

    if (it_user != userMap.end())
    {
        auto list_it = it_user->second;

        // Step 1.1: If the score is the same, no update needed
        if (list_it->score == new_score)
        {
            std::cout << "score not changed, no need to update" << std::endl;
            return;
        }

        // Step 1.2: If the new score is lower, update needed
        if (list_it->score < new_score)
        {
            auto next_it = std::next(list_it, 1);                  // Get forward iterator
            auto reverse_it = std::make_reverse_iterator(list_it); // Get reverse iterator

            // Step 1.2.4: Update ranks for equal scores in reverse order
            while (reverse_it != rankList.rend() && reverse_it->score == list_it->score)
            {
                reverse_it->rank++;
                reverse_it++;
            }

            // Step 1.2.5: Remove elements beyond rank 500
            if (reverse_it != rankList.rend() && reverse_it->rank > 500)
            {
                auto erase_it = reverse_it.base();
                rankList.erase(erase_it, rankList.end());
                for (auto it = erase_it; it != rankList.end(); ++it)
                {
                    userMap.erase(it->charid);
                }
            }

            unsigned int tmp_rank = 1; // Initialize temporary rank

            // Step 1.2.6: Forward scan to find correct insertion point
            while (reverse_it != rankList.rend())
            {
                if (reverse_it->score < new_score)
                {
                    reverse_it->rank++;
                }
                else if (reverse_it->score == new_score)
                {
                    tmp_rank = reverse_it->rank;
                    break;
                }
                else
                {
                    // Same score group handling
                    auto forward_it = reverse_it.base();
                    unsigned int n = 0;
                    while (forward_it != rankList.end() && forward_it->score == reverse_it->score)
                    {
                        n++;
                        forward_it++;
                    }
                    tmp_rank = reverse_it->rank + n;
                    break;
                }
                reverse_it++;
            }

            // Step 1.2.7: Insert new rankitem in the correct position
            auto it_insert = rankList.insert(reverse_it.base(), rankitem{charid, new_score, tmp_rank});
            userMap[charid] = it_insert;

            // Step 1.2.8: Erase the old iterator
            rankList.erase(list_it);
        }
    }
    else
    {
        // Step 2: If the user is not in the leaderboard
        auto reverse_it = rankList.rbegin();
        unsigned int tmp_rank = 1;
        auto toErase = rankList.end();

        while (reverse_it != rankList.rend())
        {
            if (reverse_it->score < new_score)
            {
                reverse_it->rank++;
                if (reverse_it->rank > 500)
                {
                    if (toErase == rankList.end())
                    {
                        toErase = reverse_it.base();
                    }
                }
            }
            else if (reverse_it->score == new_score)
            {
                tmp_rank = reverse_it->rank;
                break;
            }
            else
            {
                // Same score group handling
                auto forward_it = reverse_it.base();
                unsigned int n = 0;
                while (forward_it != rankList.end() && forward_it->score == reverse_it->score)
                {
                    n++;
                    forward_it++;
                }
                tmp_rank = reverse_it->rank + n;
                break;
            }
            reverse_it++;
        }

        // Step 2.4: Remove elements beyond rank 500
        if (toErase != rankList.end())
        {
            rankList.erase(toErase, rankList.end());
            for (auto it = toErase; it != rankList.end(); ++it)
            {
                userMap.erase(it->charid);
            }
        }

        // Step 2.5: Insert the new user
        auto it_insert = rankList.insert(reverse_it.base(), rankitem{charid, new_score, tmp_rank});
        userMap[charid] = it_insert;
    }
}

int main()
{
    // 示例使用
    updateLeaderboard(1, 100);
    updateLeaderboard(2, 150);
    updateLeaderboard(1, 120);
    updateLeaderboard(3, 90);

    for (const auto &item : rankList)
    {
        std::cout << "CharID: " << item.charid << ", Score: " << item.score << ", Rank: " << item.rank << std::endl;
    }

    return 0;
}
