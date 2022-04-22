#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "omp.h"

using namespace std;

double determinate_omp(vector <vector <double>> matrix, int threads) {
    int size = matrix.size();
    double determinant = 1;
    for (int i = 0; i < size; i++) {
        int g = -1;
#pragma omp parallel for num_threads(threads)
        for (int j = i; j < size; j++) {
            if (matrix[j][i] != 0 && g == -1) {
                g = j;
            }
        }
        if (g == -1) {
            determinant = 0;
            break;
        }
        if (g != i) {
            swap(matrix[i], matrix[g]);
            determinant *= -1;
        }
#pragma omp parallel for num_threads(threads)
        for (int j = i + 1; j < size; j++) {
            double k = matrix[j][i] / matrix[i][i];
            for (int y = i; y < size; y++) {
                matrix[j][y] -= k * matrix[i][y];
            }
        }
    }
    for (int i = 0; i < size; i++) {
        determinant *= matrix[i][i];
    }
    return determinant;
}

double determinate_without(vector <vector <double>> matrix);

int parse(string s) {
    if (s[0] == '-') {
        return -1;
    }
    int n = 0;
    for (int i = 0; i < s.length(); i++) {
        n = n * 10 + s[i] - '0';
    }
    return n;
}

int main(int argc, char* argv[]) {
    int threads = parse(argv[1]);
    if (threads < 0) {
        cout << "Num of threads less 0";
        return 0;
    }
    ifstream in(argv[2]);
    if (!in.is_open()) {
        cout << "Input file doesn't open";
        return 0;
    }
    int n;
    in >> n;
    vector <vector <double>> matrix(n, vector <double> (n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            in >> matrix[i][j];
    }
    in.close();
    clock_t clockT = clock();
    double det = determinate_without(matrix);
    double time = ((double) clock() - (double) clockT) / CLOCKS_PER_SEC;
    if (argc > 3) {
        ofstream out(argv[3]);
        if (!out.is_open()) {
            cout << "Output file doesn't open";
            return 0;
        }
        out << fixed << setprecision(7) << "Determinant: " <<  det << endl;
        out.close();
    } else {
        cout << fixed << setprecision(7) << "Determinant: " <<  det << endl;
    }
    cout << fixed << setprecision(7) << "Time " << time * 1000 << " ms" << endl;
    double start_time;
    start_time = omp_get_wtime();
    double det_omp = determinate_omp(matrix, threads);
    cout << fixed << setprecision(7) << "Time (" << threads << " thread(s)): "
         << (omp_get_wtime() - start_time) * 1000 << " ms" << endl;
    cout << "Determinate parallel: " << det_omp << endl; 
    return 0;
}

double determinate_without(vector <vector <double>> matrix) {
    int size = matrix.size();
    double determinant = 1;
    for (int i = 0; i < size; i++) {
        int g = -1;
        for (int j = i; j < size; j++) {
            if (matrix[j][i] != 0 && g == -1) {
                g = j;
            }
        }
        if (g == -1) {
            determinant = 0;
            break;
        }
        if (g != i) {
            swap(matrix[i], matrix[g]);
            determinant *= -1;
        }
        for (int j = i + 1; j < size; j++) {
            double k = matrix[j][i] / matrix[i][i];
            for (int y = i; y < size; y++) {
                matrix[j][y] -= k * matrix[i][y];
            }
        }
    }
    for (int i = 0; i < size; i++) {
        determinant *= matrix[i][i];
    }
    return determinant;
}
