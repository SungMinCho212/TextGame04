#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// PotionRecipe 클래스: 재료 목록을 vector<string>으로 변경
class PotionRecipe {
public:
    std::string potionName;
    std::vector<std::string> ingredients; // 단일 재료에서 재료 '목록'으로 변경

    // 생성자: 재료 목록을 받아 초기화하도록 수정
    PotionRecipe(const std::string& name, const std::vector<std::string>& ingredients)
        : potionName(name), ingredients(ingredients) {
    }
};

// AlchemyWorkshop 클래스: 레시피 목록을 관리
class AlchemyWorkshop {
private:
    std::vector<PotionRecipe> recipes;

    std::unordered_map<std::string, int> stock;
    std::unordered_map<std::string, int> issued;

    
    size_t findIndexByName(const std::string& name) const 
    {
        for (size_t i = 0; i < recipes.size(); ++i) 
        {
            if (recipes[i].potionName == name) return i;
        }
        return static_cast<size_t>(-1);
    }

   
    void printRecipe(const PotionRecipe& r) const 
    {
        std::cout << "- 물약 이름: " << r.potionName << "\n  > 필요 재료: ";
        for (size_t j = 0; j < r.ingredients.size(); ++j) 
        {
            std::cout << r.ingredients[j];
            if (j + 1 < r.ingredients.size()) std::cout << ", ";
        }
        std::cout << std::endl;
    }


public:
    // addRecipe 메서드: 재료 목록(vector)을 매개변수로 받도록 수정
    void addRecipe(const std::string& name, const std::vector<std::string>& ingredients) 
    {
        if (findIndexByName(name) != static_cast<size_t>(-1))
        {
            std::cout << "이미 존재하는 물약 입니다." << std::endl;
            return;
        }

        recipes.push_back(PotionRecipe(name, ingredients));
        stock[name] = 3;
        issued[name] = 0;
        std::cout << ">> 새로운 레시피 '" << name << "'이(가) 추가되었습니다." << std::endl;
    }

    // 모든 레시피 출력 메서드
    void displayAllRecipes() const 
    {
        if (recipes.empty()) {
            std::cout << "아직 등록된 레시피가 없습니다." << std::endl;
            return;
        }

        std::cout << "\n--- [ 전체 레시피 목록 ] ---" << std::endl;
        for (size_t i = 0; i < recipes.size(); ++i) 
        {
            printRecipe(recipes[i]);

            
        }
        std::cout << "---------------------------\n";

    }

    void searchByName(const std::string& name) const
    {
        size_t idx = findIndexByName(name);
        if (idx == static_cast<size_t>(-1))
        {
            std::cout << "해당하는 이름의 물약을 찾을 수 없습니다." << std::endl;
            return;
        }
        std::cout << "\n --- [ 검색결과 : 이름 ] ---" << std::endl;
        printRecipe(recipes[idx]);
        std::cout << "---------------------------------\n";

    }

    void searchByIngredient(const std::string& ingredient) const
    {
        bool found = false;
        std::cout << "\n === [ 검색 결과 : 재료 ' ===" << ingredient << "' 포함 ] === " << std::endl;
        for (const auto& r : recipes)
        {
            if (std::find(r.ingredients.begin(), r.ingredients.end(), ingredient) != r.ingredients.end())
            {
                printRecipe(r);
                found = true;
            }
        }
        if (!found)
        {
            std::cout << "해당 재료를 포함한 레시피가 없습니다." << std::endl;
        }
        std::cout << "==========================\n";
    }

    void showStock(const std::string& name) const 
    {
        if (stock.find(name) == stock.end()) 
        {
            std::cout << "해당 이름의 물약이 존재하지 않습니다." << std::endl;
            return;
        }
        int curStock = stock.at(name);
        int curIssued = issued.at(name);
        std::cout << "물약 '" << name << "' 재고: " << curStock << " (지급 중: " << curIssued << ", 최대: 3)" << std::endl;
    }

    void givePotion(const std::string& name)
    {
        if (stock.find(name) == stock.end()) 
        {
            std::cout << "해당 이름의 물약이 존재하지 않습니다." << std::endl;
            return;
        }
        int& curStock = stock[name];
        int& curIssued = issued[name];

        if (curStock <= 0) {
            std::cout << "재고가 부족합니다. 지급할 수 없습니다." << std::endl;
            return;
        }
        curStock -= 1;
        curIssued += 1;
        std::cout << ">> '" << name << "' 1개 지급 완료. 현재 재고: " << curStock << " (지급 중: " << curIssued << ")" << std::endl;
    }

    void returnEmptyBottle(const std::string& name) 
    {
        if (stock.find(name) == stock.end()) 
        {
            std::cout << "해당 이름의 물약이 존재하지 않습니다." << std::endl;
            return;
        }
        int& curStock = stock[name];
        int& curIssued = issued[name];

        if (curIssued <= 0) {
            std::cout << "지급 중인 물약이 없습니다. 반환을 처리할 수 없습니다." << std::endl;
            return;
        }
        if (curStock >= 3) {
            std::cout << "현재 재고가 이미 최대치(3)입니다. 반환을 처리할 수 없습니다." << std::endl;
            return;
        }
        curStock += 1;
        curIssued -= 1;
        std::cout << ">> '" << name << "' 공병 반환 처리. 현재 재고: " << curStock << " (지급 중: " << curIssued << ")" << std::endl;
    }
};


int main() {
    AlchemyWorkshop myWorkshop;

    while (true) {
        std::cout << "\n연금술 공방 관리 시스템" << std::endl;
        std::cout << "1. 레시피 추가" << std::endl;
        std::cout << "2. 모든 레시피 출력" << std::endl;
        std::cout << "3. 물약 이름으로 검색" << std::endl;
        std::cout << "4. 재료로 검색" << std::endl;
        std::cout << "5. 재고 조회" << std::endl;
        std::cout << "6. 물약 지급" << std::endl;
        std::cout << "7. 공병 반환" << std::endl;
        std::cout << "8. 종료" << std::endl;
        std::cout << "선택: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            std::string name;
            std::cout << "물약 이름: ";
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, name);

            std::vector<std::string> ingredients_input;
            std::string ingredient;
            std::cout << "필요한 재료들을 입력하세요. (입력 완료 시 '끝' 입력)" << std::endl;
            while (true) {
                std::cout << "재료 입력: ";
                std::getline(std::cin, ingredient);
                if (ingredient == "끝") break;
                if (!ingredient.empty()) ingredients_input.push_back(ingredient);
            }

            if (!ingredients_input.empty()) {
                myWorkshop.addRecipe(name, ingredients_input);
            }
            else {
                std::cout << "재료가 입력되지 않아 레시피 추가를 취소합니다." << std::endl;
            }

        }
        else if (choice == 2) {
            myWorkshop.displayAllRecipes();

        }
        else if (choice == 3) {
            std::cout << "검색할 물약 이름: ";
            std::cin.ignore(10000, '\n');
            std::string name;
            std::getline(std::cin, name);
            myWorkshop.searchByName(name);

        }
        else if (choice == 4) {
            std::cout << "검색할 재료 이름: ";
            std::cin.ignore(10000, '\n');
            std::string ing;
            std::getline(std::cin, ing);
            myWorkshop.searchByIngredient(ing);

        }
        else if (choice == 5) {
            std::cout << "재고를 확인할 물약 이름: ";
            std::cin.ignore(10000, '\n');
            std::string name;
            std::getline(std::cin, name);
            myWorkshop.showStock(name);

        }
        else if (choice == 6) {
            std::cout << "지급할 물약 이름: ";
            std::cin.ignore(10000, '\n');
            std::string name;
            std::getline(std::cin, name);
            myWorkshop.givePotion(name);

        }
        else if (choice == 7) {
            std::cout << "반환 처리할 물약 이름: ";
            std::cin.ignore(10000, '\n');
            std::string name;
            std::getline(std::cin, name);
            myWorkshop.returnEmptyBottle(name);

        }
        else if (choice == 8) {
            std::cout << "공방 문을 닫습니다..." << std::endl;
            break;

        }
        else {
            std::cout << "잘못된 선택입니다. 다시 시도하세요." << std::endl;
        }
    }

    return 0;
}