#include "item/EnchantedBook‌.h"
#include "status/StatusVector.hpp"
#include <cmath>
#include <format>
#include <iostream>
#include <string>
#include <Windows.h>
enum class Step {
    computerAnvilUsagePenalty,
    experienceForRenameOnly,
    enchantmentMerge,
    calculateExperienceToLevel,
    exhaustiveEnchantOptimizer‌,
    exit
};
Step intToStep(int i) {
    switch (i) {
        case 1: {
            return Step::computerAnvilUsagePenalty;
        }
        case 2: {
            return Step::experienceForRenameOnly;
        }
        case 3: {
            return Step::enchantmentMerge;
        }
        case 4: {
            return Step::calculateExperienceToLevel;
        }
        case 5: {
            return Step::exhaustiveEnchantOptimizer‌;
        }
        default: {
            return Step::exit;
        }
    }
}
Step beginSelect() {
    std::cout << "0：退出" << std::endl;
    std::cout << "1：根据重命名所需经验来反推操作数" << std::endl;
    std::cout << "2：已知操作数计算仅重命名所需经验" << std::endl;
    std::cout << "3：合并附魔" << std::endl;
    std::cout << "4：计算0级到指定等级所需经验值" << std::endl;
    std::cout << "5：穷举法计算最少经验需求附魔路线" << std::endl;
    std::cout << "请选择：" << std::endl;
    int input;
    std::cin >> input;
    return intToStep(input);
}
void computerAnvilUsagePenalty() {
    std::cout << "请输入重命名所需等级：" << std::endl;
    int level;
    std::cin >> level;
    std::cout << "操作数：" << log(level) / log(2) << std::endl;
}
void experienceForRenameOnly() {
    std::cout << "请输入操作数：" << std::endl;
    int usage;
    std::cin >> usage;
    std::cout << "需要等级：" << pow(2, usage) << std::endl;
}
void enchantmentMerge() {
    std::cout << "请输入魔咒数：" << std::endl;
    int count;
    std::cin >> count;
    int costLevel = 0;
    for (int i = 0; i < count; i++) {
        std::cout << "请输入第" << i + 1 << "个魔咒的合并魔咒花费乘数：" << std::endl;
        int enchantmentMergeCostMultiplier;
        std::cin >> enchantmentMergeCostMultiplier;
        std::cout << "请输入第" << i + 1 << "个魔咒的附魔等级：" << std::endl;
        int enchantmentLevel;
        std::cin >> enchantmentLevel;
        costLevel += enchantmentMergeCostMultiplier * enchantmentLevel;
    }
    std::cout << "请输入目标物品的操作数：" << std::endl;
    int leftUsage;
    std::cin >> leftUsage;
    std::cout << "请输入牺牲物品的操作数：" << std::endl;
    int rightUsage;
    std::cin >> rightUsage;
    std::cout << "需要等级：" << costLevel + pow(2, leftUsage) - 1 + pow(2, rightUsage) - 1 << std::endl;
}
void calculateExperienceToLevel() {
    std::cout << "请输入等级：" << std::endl;
    int level;
    std::cin >> level;
    int cost = 0;
    if (0 <= level && level <= 16) {
        cost = level * level + 6 * level;
    } else if (17 <= level && level <= 31) {
        cost = 2.5 * level * level - 40.5 * level + 360;
    } else if (32 <= level) {
        cost = 4.5 * level * level - 162.5 * level + 2220;
    }
    std::cout << "经验值：" << cost << std::endl;
}
int levelToExperience(int level) {
    int cost = 0;
    if (0 <= level && level <= 16) {
        cost = level * level + 6 * level;
    } else if (17 <= level && level <= 31) {
        cost = 2.5 * level * level - 40.5 * level + 360;
    } else if (32 <= level) {
        cost = 4.5 * level * level - 162.5 * level + 2220;
    }
    return cost;
}
int minCostLevel = 2147483647;
int minCostLevelForExperience = 0;
std::vector<std::vector<EnchantedBook‌>> minCostLevelForBookUseList;
int minCostExperience = 2147483647;
int minCostExperienceForLevel = 0;
std::vector<std::vector<EnchantedBook‌>> minCostExperienceForBookUseList;
std::tuple<int, int, std::vector<std::vector<int>>> computerEnchantmentCostLevel(std::vector<std::vector<EnchantedBook‌>> &bookUseList) {
    std::vector<EnchantedBook‌> bookList;
    int totleCostLevel = 0;
    int totleExperience = 0;
    std::vector<std::vector<int>> costLevelVector(bookUseList.size());
    for (size_t i = 0; i < bookUseList.size(); i++) {
        int leftUsage = bookUseList[i][0].usage;
        EnchantedBook‌ book {};
        for (int k = 0; k < bookUseList[i][0].enchants.size(); k++) {
            book.enchants.push_back(bookUseList[i][0].enchants[k]);
        }
        for (size_t j = 1; j < bookUseList[i].size(); j++) {
            int costLevel = 0;
            for (int k = 0; k < bookUseList[i][j].enchants.size(); k++) {
                book.enchants.push_back(bookUseList[i][j].enchants[k]);
                costLevel += bookUseList[i][j].enchants[k].costMultiplier * bookUseList[i][j].enchants[k].level;
            }
            costLevel += pow(2, leftUsage) - 1 + pow(2, bookUseList[i][j].usage) - 1;
            totleCostLevel += costLevel;
            totleExperience += levelToExperience(costLevel);
            costLevelVector[i].push_back(costLevel);
            leftUsage = max(leftUsage, bookUseList[i][j].usage) + 1;
        }
        book.usage = leftUsage;
        bookList.push_back(book);
    }
    int leftUsage = 0;
    for (size_t i = 0; i < bookList.size(); i++) {
        int costLevel = 0;
        for (int k = 0; k < bookList[i].enchants.size(); k++) {
            costLevel += bookList[i].enchants[k].costMultiplier * bookList[i].enchants[k].level;
        }
        costLevel += pow(2, leftUsage) - 1 + pow(2, bookList[i].usage) - 1;
        totleCostLevel += costLevel;
        totleExperience += levelToExperience(costLevel);
        costLevelVector[i].push_back(costLevel);
        leftUsage = max(leftUsage, bookList[i].usage) + 1;
    }
    return std::make_tuple(totleCostLevel, totleExperience, costLevelVector);
}
bool enchantment(StatusVector<EnchantedBook‌> &bookList, std::vector<std::vector<EnchantedBook‌>> &bookUseList);
void generatePermutations(StatusVector<EnchantedBook‌> &bookList, int m, std::vector<std::vector<EnchantedBook‌>> &bookUseList) {
    if (m == bookUseList[bookUseList.size() - 1].size()) {
        if (enchantment(bookList, bookUseList)) {
            auto [totleCostLevel, totleExperience, costLevel] = computerEnchantmentCostLevel(bookUseList);
            if (totleCostLevel < minCostLevel || (totleCostLevel == minCostLevel && minCostLevelForExperience > totleExperience)) {
                minCostLevel = totleCostLevel;
                minCostLevelForExperience = totleExperience;
                minCostLevelForBookUseList = bookUseList;
            }
            if (totleExperience < minCostExperience || (totleExperience == minCostExperience && minCostExperienceForLevel > totleCostLevel)) {
                minCostExperienceForLevel = totleCostLevel;
                minCostExperience = totleExperience;
                minCostExperienceForBookUseList = bookUseList;
            }
            // std::cout << "方案消耗等级：" << totleCostLevel << " 方案消耗经验值：" << totleExperience << std::endl;
            // for (size_t i = 0; i < bookUseList.size(); i++) {
            // std::string idList;
            // std::string levelList;
            // for (size_t j = 0; j < bookUseList[i].size(); j++) {
            // idList += std::to_string(bookUseList[i][j].id) + " ";
            // }
            // for (size_t j = 0; j < costLevel[i].size(); j++) {
            // levelList += std::to_string(costLevel[i][j]) + " ";
            // }
            // std::cout << std::format("{:<20} {}", idList, levelList) << std::endl;
            // }
        }
        return;
    }
    for (size_t i = 0; i < bookList.size(); i++) {
        if (bookList[i].getValid()) {
            bookList[i].setValid(false);
            bookUseList[bookUseList.size() - 1].push_back(bookList[i].getData());
            generatePermutations(bookList, m, bookUseList);
            bookUseList[bookUseList.size() - 1].pop_back();
            bookList[i].setValid(true);
        }
    }
}
bool enchantment(StatusVector<EnchantedBook‌> &bookList, std::vector<std::vector<EnchantedBook‌>> &bookUseList) {
    int remaining = 0;
    for (size_t i = 0; i < bookList.size(); i++) {
        if (bookList[i].getValid()) {
            remaining++;
        }
    }
    if (remaining == 0) {
        return true;
    }
    for (size_t i = 0; i < bookList.size(); i++) {
        if (bookList[i].getValid()) {
            bookUseList.push_back(std::vector<EnchantedBook‌>());
            generatePermutations(bookList, remaining--, bookUseList);
            bookUseList.pop_back();
        }
    }
    return false;
}
void exhaustiveEnchantOptimizer‌() {
    int id = 0;
    std::cout << "请输入魔咒数：" << std::endl;
    int count;
    std::cin >> count;
    StatusVector<EnchantedBook‌> bookList;
    bookList.reserve(count);
    for (int i = 0; i < count; i++) {
        EnchantedBook‌ book {};
        Enchant enchant {};
        std::cout << "请输入第" << i + 1 << "个魔咒的合并魔咒花费乘数：" << std::endl;
        std::cin >> enchant.costMultiplier;
        std::cout << "请输入第" << i + 1 << "个魔咒的附魔等级：" << std::endl;
        std::cin >> enchant.level;
        book.id = id++;
        book.enchants.push_back(enchant);
        bookList.push_back(book);
    }
    minCostLevel = 2147483647;
    minCostExperience = 2147483647;
    std::vector<std::vector<EnchantedBook‌>> bookUseList {};
    enchantment(bookList, bookUseList);
    if (minCostLevel <= minCostExperienceForLevel && minCostLevelForExperience == minCostExperience) {
        std::cout << "最佳方案消耗等级：" << minCostLevel << " 方案消耗经验值：" << minCostLevelForExperience << std::endl;
        auto [totleCostLevel, totleExperience, costLevel] = computerEnchantmentCostLevel(minCostLevelForBookUseList);
        for (size_t i = 0; i < minCostLevelForBookUseList.size(); i++) {
            std::string idList;
            std::string levelList;
            for (size_t j = 0; j < minCostLevelForBookUseList[i].size(); j++) {
                idList += std::to_string(minCostLevelForBookUseList[i][j].id) + " ";
            }
            for (size_t j = 0; j < costLevel[i].size(); j++) {
                levelList += std::to_string(costLevel[i][j]) + " ";
            }
            std::cout << std::format("{:<20} {}", idList, levelList) << std::endl;
        }
    } else if (minCostLevel == minCostExperienceForLevel && minCostLevelForExperience >= minCostExperience) {
        std::cout << "最佳方案消耗等级：" << minCostExperienceForLevel << " 方案消耗经验值：" << minCostExperience << std::endl;
        auto [totleCostLevel, totleExperience, costLevel] = computerEnchantmentCostLevel(minCostExperienceForBookUseList);
        for (size_t i = 0; i < minCostExperienceForBookUseList.size(); i++) {
            std::string idList;
            std::string levelList;
            for (size_t j = 0; j < minCostExperienceForBookUseList[i].size(); j++) {
                idList += std::to_string(minCostExperienceForBookUseList[i][j].id) + " ";
            }
            for (size_t j = 0; j < costLevel[i].size(); j++) {
                levelList += std::to_string(costLevel[i][j]) + " ";
            }
            std::cout << std::format("{:<20} {}", idList, levelList) << std::endl;
        }
    } else {
        {
            std::cout << "最少等级方案消耗等级：" << minCostLevel << " 方案消耗经验值：" << minCostLevelForExperience << std::endl;
            auto [totleCostLevel, totleExperience, costLevel] = computerEnchantmentCostLevel(minCostLevelForBookUseList);
            for (size_t i = 0; i < minCostLevelForBookUseList.size(); i++) {
                std::string idList;
                std::string levelList;
                for (size_t j = 0; j < minCostLevelForBookUseList[i].size(); j++) {
                    idList += std::to_string(minCostLevelForBookUseList[i][j].id) + " ";
                }
                for (size_t j = 0; j < costLevel[i].size(); j++) {
                    levelList += std::to_string(costLevel[i][j]) + " ";
                }
                std::cout << std::format("{:<20} {}", idList, levelList) << std::endl;
            }
        }
        {
            std::cout << "最少经验方案消耗等级：" << minCostExperienceForLevel << " 方案消耗经验值：" << minCostExperience << std::endl;
            auto [totleCostLevel, totleExperience, costLevel] = computerEnchantmentCostLevel(minCostExperienceForBookUseList);
            for (size_t i = 0; i < minCostExperienceForBookUseList.size(); i++) {
                std::string idList;
                std::string levelList;
                for (size_t j = 0; j < minCostExperienceForBookUseList[i].size(); j++) {
                    idList += std::to_string(minCostExperienceForBookUseList[i][j].id) + " ";
                }
                for (size_t j = 0; j < costLevel[i].size(); j++) {
                    levelList += std::to_string(costLevel[i][j]) + " ";
                }
                std::cout << std::format("{:<20} {}", idList, levelList) << std::endl;
            }
        }
    }
}
int main(int argc, char **argv) {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "mc附魔测试工具" << std::endl;
    auto step = beginSelect();
    while (step != Step::exit) {
        switch (step) {
            case Step::computerAnvilUsagePenalty: {
                computerAnvilUsagePenalty();
                break;
            }
            case Step::experienceForRenameOnly: {
                experienceForRenameOnly();
                break;
            }
            case Step::enchantmentMerge: {
                enchantmentMerge();
                break;
            }
            case Step::calculateExperienceToLevel: {
                calculateExperienceToLevel();
                break;
            }
            case Step::exhaustiveEnchantOptimizer‌: {
                exhaustiveEnchantOptimizer‌();
                break;
            }
        }
        step = beginSelect();
    }
    return 0;
}
