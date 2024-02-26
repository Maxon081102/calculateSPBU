#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_set>

using namespace std;

class Cell {
public:
    int x;
    unordered_set<int> variants;

    Cell(int x, int N) : x(x) {
        if (x == 0) {
            for (int i = 1; i <= N; ++i) {
                variants.insert(i);
            }
        }
    }

    bool empty() const {
        return x == 0;
    }

    bool isValid() const {
        return !empty() || !variants.empty();
    }

    void set(int x) {
        this->x = x;
        variants.clear();
    }

    bool isCandidate(int x) const {
        return std::find(variants.begin(), variants.end(), x) != variants.end() && empty();
    }

    bool isObvious() const {
        return empty() && variants.size() == 1;
    }
};

class Field {
public:
    vector<vector<Cell>> cells;
    int N;
    int n;
    int freeCells;

    Field(int N, int n) : N(N), n(n), freeCells(0) {
        cells = vector<vector<Cell>>(N, vector<Cell>(N, Cell(0, N)));

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (cells[i][j].empty()) {
                    ++freeCells;
                }
            }
        }
    }

    Field(const Field& other) : 
    N(other.N), n(other.n), freeCells(other.freeCells), cells(other.cells) {};

    bool isValid() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!cells[i][j].isValid()) {
                    return false;
                }
            }
        }
        return true;
    }

    int updateCell(int i, int j, int x) {
        cells[i][j].variants.erase(x);
        if (cells[i][j].isObvious()) {
            return 1;
        }
        if (!cells[i][j].isValid()) {
            return 2;
        }
        return 0;
    }

    int set(int i, int j, int x) {
        if (x == 0) {
            return 0;
        }
        int code = 0;
        freeCells--;
        cells[i][j].set(x);

        for (int k = 0; k < N; ++k) {
            code = updateCell(i, k, x);
            if (code == 2) {
                return 2;
            }
            code = updateCell(k, j, x);
            if (code == 2) {
                return 2;
            }
        }

        int start_y = i / n * n;
        int start_x = j / n * n;
        for (int k = start_y; k < start_y + n; ++k) {
            for (int z = start_x; z < start_x + n; ++z) {
                code = updateCell(k, z, x);
                if (code == 2) {
                    return 2;
                }
            }
        }

        return code;
    }

    void resolveSimpleCells() {
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (!cells[i][j].isObvious()) {
                        continue;
                    }
                    int code = set(i, j, *cells[i][j].variants.begin());
                    if (code == 2) {
                        return;
                    }
                    changed = true;
                }
            }
        }
    }

    void print() {
        cout << "----- FIELD -----" << endl;
        for (int i = 0; i < N ; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << cells[i][j].x << " ";
            }
            cout << endl;
        }
    }
};

bool operator<(const Field& first, const Field& second) {
    if (first.freeCells == second.freeCells) {
        return true;
    }
    return first.freeCells > second.freeCells;
}

void findBestCell(Field& f, int& best_i, int& best_j, int N) {
    int minSize = N + 1;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (f.cells[i][j].empty() && f.cells[i][j].variants.size() < minSize) {
                minSize = f.cells[i][j].variants.size();
                best_i = i;
                best_j = j;
            }
        }
    }
}

int findSolution(vector<Field> heap, int N) {
    while (!heap.empty()) {
        pop_heap(heap.begin(), heap.end());
        auto field = heap.back();
        heap.pop_back();

        if (field.freeCells == 0) {
            field.print();
            return 0;
        }

        int best_i = 0;
        int best_j = 0;
        findBestCell(field, best_i, best_j, N);

        for (auto x : field.cells[best_i][best_j].variants) {
            Field new_field = field;
            int code = new_field.set(best_i, best_j, x);
            if (code == 1) {
                new_field.resolveSimpleCells();
            }
            if (code != 2 && new_field.isValid()) {
                heap.push_back(new_field);
                push_heap(heap.begin(), heap.end());
            }
        }
    }
    return 1;
}

int main() {
    int N;
    int n;
    int number;
    cin >> N;
    n = sqrt(N);

    Field startField(N, n);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> number;
            startField.set(i, j, number);
        }
    }
    startField.resolveSimpleCells();

    vector<Field> heap;
    heap.push_back(startField);
    int code = findSolution(heap, N);
    if (code != 0) {
        cout << "Invalid field" << endl;
    }
    return 0;
}