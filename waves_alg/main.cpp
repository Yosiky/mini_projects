#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>

struct pair_four {
    int first;  // координата x
    int second; // координата y
    int third;  // направление (с какой стороны мы пришли)
    int fourth; // номер волны
};

// направления
enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};

char    g_sym_wall = '#'; // не работает пока
char    g_sym_void = '_'; // не работает пока
char    g_start_point = 'A';
char    g_finish_point = 'B';
std::vector<std::vector<int>>   visit;
std::queue<pair_four> que;

void check_map(std::vector<std::string> &map) {
    int max_x;

    if (map.size() == 0) {
        std::cerr << "Error: map is void" << std::endl;
        exit(1);
    }
    max_x = map[0].size();
    for (const std::string &line: map) {
        if (max_x != line.size()) {
            std::cerr << "Error: string lenght is not equal" << std::endl;
            exit(1);
        }
        for (int i = 0; i < line.size(); ++i) {
            if (!(line[i] == g_sym_void || line[i] == g_sym_wall)) {
                std::cerr << "Error: symbol in map is not correct" << std::endl;
                exit(1);
            }
        }
    }
}

int set_point(std::vector<std::string> &map, const std::pair<int, int> &point, char sym) {
    int max_y = map.size();
    int max_x = map[0].size();

    if (point.first < 0 || max_x <= point.first || point.second < 0 || max_y <= point.second) {
        std::cout << "Error: invalid point. Exit for bounds" << std::endl;
        return (1);
    }
    if (map[point.second][point.first] == '_') {
        std::cout << "Write point" << std::endl;
        map[point.second][point.first] = sym;
    }
    else {
        std::cerr << "Error: invalid point. There is wall or start symbol" << std::endl;
        return (1);
    }
    return (0);
}

void print(std::vector<std::string> &map, bool on_num=0) {
    std::cout << " ";
    for (int i = 0; i < map[0].size(); ++i)
        std::cout << " | " << i << "\t";
    std::cout << " |" << std::endl << "--";
    for (int i = 0; i < map[0].size(); ++i)
        std::cout << "--------";
    std::cout << std::endl;
    for (int i = 0; i < map.size(); ++i) {
        std::cout << i << " |\t";
        for (int j = 0; j < map[0].size(); ++j) {
            if (on_num) {
                if (map[i][j] == g_start_point)
                    std::cout << g_start_point << '\t';
                else if (map[i][j] == g_finish_point)
                    std::cout << g_finish_point << '\t';
                else if (map[i][j] == g_sym_wall)
                    std::cout << g_sym_wall << "\t";
                else if (visit[i][j] == INT_MAX)
                    std::cout << g_sym_void << "\t";
                else
                    std::cout << visit[i][j] << "\t";
            }
            else {
                std::cout << map[i][j] << '\t' ;
            }
        }
        std::cout << std::endl;
    }
}

bool is_valid_coordinate(const std::pair<int, int> &point, int width, int hight) {
    if (0 <= point.first && point.first < width
        && 0 <= point.second && point.second < hight)
        return (true);
    return (false);
}

int bfs(std::vector<std::string> &map, std::pair<int, int> &start_point, std::pair<int, int> &finish_point) {
    int                 width_line = map[0].size(); // ширина карты
    int                 hight_line = map.size();    // высота карты
    int                 last_value = INT_MAX;       // откуда пришли
    bool                add = 1;                    // добавляем элемент
    pair_four           value;                      // значение из очереди
    int                 num_wave = 0;               // номер волны

    visit.resize(map.size());
    for (std::vector<int> &line : visit)
        line.resize(width_line, INT_MAX);
    visit[start_point.second][start_point.first] = 0;
    que.push({start_point.first, start_point.second, NONE, num_wave});
    while (!que.empty()) {
        value = que.front(); // считывание первого элемента из очереди
        que.pop(); // удаление элемента из очереди
        if (num_wave != value.fourth) { // вывод новой волны
            std::cout << "Number wave: " << num_wave << std::endl;
            print(map, 1);
            num_wave = value.fourth;
        }
        if (map[value.second][value.first] == g_sym_wall)
            continue ;
        if (value.third == LEFT)
            last_value = visit[value.second][value.first + 1] + 1;
        if (value.third == RIGHT)
            last_value = visit[value.second][value.first - 1] + 1;
        if (value.third == UP)
            last_value = visit[value.second + 1][value.first] + 1;
        if (value.third == DOWN)
            last_value = visit[value.second - 1][value.first] + 1;
        if (last_value < visit[value.second][value.first]) { // выбор кратчайшего пути
            visit[value.second][value.first] = last_value;
            add = 1;
        }
        if (add && is_valid_coordinate({value.first - 1, value.second}, width_line, hight_line) && value.third != RIGHT)
            que.push({value.first - 1, value.second, LEFT, value.fourth + 1});
        if (add && is_valid_coordinate({value.first + 1, value.second}, width_line, hight_line) && value.third != LEFT)
            que.push({value.first + 1, value.second, RIGHT, value.fourth + 1});
        if (add && is_valid_coordinate({value.first, value.second - 1}, width_line, hight_line) && value.third != DOWN)
            que.push({value.first, value.second - 1, UP, value.fourth + 1});
        if (add && is_valid_coordinate({value.first, value.second + 1}, width_line, hight_line) && value.third != UP)
            que.push({value.first, value.second + 1, DOWN, value.fourth + 1});
        add = 0;
    }
    return (visit[finish_point.second][finish_point.first]);
}

int main(int argc, char **argv) {
    std::string     fileName;
    std::ifstream   fd;
    std::pair<int, int>  start;
    std::pair<int, int>  finish;
    std::vector<std::string> map;
    int answer = 0;

    std::cout << "Input file's name: "; // вводим путь до файла с картой
    std::cin >> fileName;

    fd.open(fileName); // открываем файл для чтения
    if (!fd.is_open()) { // проверяем, что файл открылся
        std::cerr << "Error: file is not open" << std::endl;
        exit(1);
    }
    for (std::string line; std::getline(fd, line); ) // считываем карту
        map.push_back(line);
    check_map(map); // проверяем карту на валидность
    print(map); // вывод карты
    do {
        std::cout << "Input start point('x y'): ";
        std::cin >> start.first >> start.second;
    } while (set_point(map, start, g_start_point)); // считываем координаты начала и проверяем на валидность
    do {
        std::cout << "Input finish point('x y'): ";
        std::cin >> finish.first >> finish.second;
    } while (set_point(map, finish, g_finish_point)); // считываем координаты конечной точки и провеяем на валидность
    answer = bfs(map, start, finish); // запуск алгоритма
    if (answer == INT_MAX)
        std::cout << "Way between point A and point B not found" << std::endl;
    else
        std::cout << "Lenght between point A and point B equal " << answer << std::endl;

    return (0);
}
