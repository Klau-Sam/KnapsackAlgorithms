#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <stack>
#include <algorithm>

using namespace std;


struct thing {
    int w; //rozmiar
    int c; //wartosc (cena)
};

int how_many_things_in_knapsack;
int cost_in_knapsack;
int weight_in_knapsack;
stack <int> combination;
thing *knapsack;

void quick_sort(double *arr1, int *arr2, int *arr3, int b, int e) {
    int i = b, j = e;
    double key = arr1[(b+e)/2];
    do {
        while (arr1[i] < key) { i++; }
        while (arr1[j] > key) { j--; }
        if (i <= j) {
            swap(arr1[i], arr1[j]);
            swap(arr2[i], arr2[j]);
            swap(arr3[i], arr3[j]);
            i++;
            j--;
        }
    } while ( i < j );
    if ( b < j ) quick_sort(arr1, arr2, arr3, b, j);
    if ( i < e ) quick_sort(arr1, arr2, arr3, i, e);

    return;
}

void read(int n, int b, thing *B) {
    srand(time(0));
    int w, c;
    for (int i = 0; i < n; i++){
        w = (rand() % b) + 1;
        c = (rand() % (2*b)) + 1;
        B[i].w = w;
        B[i].c = c;

    }
    /* B[0].w = 4;
    B[0].c = 3;
    B[1].w = 1;
    B[1].c = 5;
    B[2].w = 2;
    B[2].c = 2;
    B[3].w = 5;
    B[3].c = 7;
    B[4].w = 3;
    B[4].c = 4;*/

    return;
}

void greedy_algorithm(int n, int b, thing *B, double *S, int *weights, int *costs) {
    knapsack = new thing [n]; //plecak
    for (int i = 0; i < n; i++)
    {
        S[i] = double(B[i].c)/B[i].w;
        weights[i] = B[i].w;
        costs[i] = B[i].c;
    }

    /* for (int i = 0; i < n; i++) {
        cout << weights[i] << ", ";
    }

    cout << endl;
    for (int i = 0; i < n; i++)  {
        cout << costs[i] << ", ";
    } */

    quick_sort(S, weights, costs, 0, n-1);

    how_many_things_in_knapsack = 0;
    cost_in_knapsack = 0;
    weight_in_knapsack = 0;

    for (int i = n-1; i >= 0; i--) {
        if (weight_in_knapsack + weights[i] <= b)
        {
            knapsack[how_many_things_in_knapsack].w = weights[i];
            knapsack[how_many_things_in_knapsack].c = costs[i];
            how_many_things_in_knapsack++;
            weight_in_knapsack += weights[i];
            cost_in_knapsack += costs[i];
        };
    }
    cout << "Waga" << weight_in_knapsack << endl;
    return;
}

void BF_algorithm(int n, int b, thing *B) {
    knapsack = new thing [n];
    int *tab = new int [n], current_w = 0, current_c = 0, best_c = 0;
    for (int i = 0; i < n; i++) tab[i] = i;

    for (unsigned i = (1 << n); --i;)
    {
        for (unsigned k = 0; k < n; ++k) {
            if ((i >> k) & 1) {
                combination.push(tab[k]);
                current_w += B[k].w;
                current_c += B[k].c;
            }
        }
        if (current_c > best_c && current_w <= b) {
            how_many_things_in_knapsack = 0;
            best_c = current_c;
            cost_in_knapsack = best_c;
            while (!combination.empty()) {
                int now = combination.top();
                knapsack[how_many_things_in_knapsack].w = B[now].w;
                knapsack[how_many_things_in_knapsack].c = B[now].c;
                how_many_things_in_knapsack++;
                combination.pop();
            }
        }
        current_c = 0;
        current_w = 0;
        while (!combination.empty()) combination.pop();
    }

    delete []tab;
    return;
}

void PD_algorithm(int n, int b, thing *B, int **P) {
    knapsack = new thing [n];
    int best = 0;

    for (int i = 0; i <= n; i++ ) {
        P[i] = new int [b + 1];
        P[i][0] = 0;
    }

    for (int j = 0; j <= b; j++ ) P[0][j] = 0;

    for (int i = 1; i <= n; i++ ){
        for (int j = 1; j <= b; j++ ) {
            if ( j >= B[i-1].w && ( P[i-1][j] < (P[i-1][j-B[i-1].w] + B[i-1].c)) ) {
                P[i][j] = P[i-1][j-B[i-1].w] + B[i-1].c;
                best = max(best, P[i][j]);
            }
            else {
                P[i][j] = P[i-1][j];
                best = max(best, P[i][j]);
            }
        }
    }

    int ii = n, jj = b;
    how_many_things_in_knapsack = 0;
    while (ii > 0) {
        if (P[ii][jj] == P[ii -1][jj]) {
            ii--;
            continue;
        }
        else {
            knapsack[how_many_things_in_knapsack].w = B[ii-1].w;
            knapsack[how_many_things_in_knapsack].c = B[ii-1].c;
            jj -= B[ii-1].w;
            ii--;
            how_many_things_in_knapsack++;
        }
    }
    cout << best << endl;
    return;
}

void clear_knapsack() {
    if (knapsack)
    {
        delete[] knapsack;
        knapsack = NULL;
    }

    return;
}

int main() {

    srand(time(NULL));

    clock_t t1, t2, find1, t3, t4, find2, t5, t6, find3;
    double seconds1, seconds2, seconds3;

    // int n = 5; // ilosc rzeczy, ilosc_plecak
    // int b = 10; // pojemnosc plecaka

    for (int n = 3000; n <= 45000;  n += 3000) {
        int b = (rand() % 300) + n;
        cout << b << endl;

        thing *B = new thing [n]; //lista z rzeczami (rozmiary, wagi)

        // do zachłannego
        double *S = new double [n];
        int *weights = new int [n], *costs = new int [n];

        // do dynamicznego
        int ** P = new int * [n + 1];

        read(n, b, B);


        t1 = clock();
        greedy_algorithm(n, b, B, S, weights, costs);
        t2 = clock();
        // cout << "Ile rzeczy " << how_many_things_in_knapsack << endl;



        /* find1 = t2 - t1;
        seconds1 = (double) find1 / CLOCKS_PER_SEC;
        cout << seconds1 << endl;
        plik1 << seconds1 << endl; */


        // do zachłannego
        /* cout << endl;
        for (int i = n-1; i >= 0; i--) {
            cout << S[i] << ", ";
        }

        cout << endl;
        for (int i = n-1; i >= 0; i--) {
            cout << weights[i] << ", ";
        }

        cout << endl;
        for (int i = n-1; i >= 0; i--)  {
            cout << costs[i] << ", ";
        }
        cout << endl << how_many_things_in_knapsack << endl;

        for (int i = 0; i < how_many_things_in_knapsack; i++)
        {
            cout << "[" << knapsack[i].w << "|" << knapsack[i].c << "] ";
        }
        cout << endl; */
        clear_knapsack();



        /* t3 = clock();
        BF_algorithm(n, b, B);
        t4 = clock();

        find2 = t4 - t3;
        seconds2 = (double) find2 / CLOCKS_PER_SEC;
        cout << seconds2 << endl;
        plik1 << seconds2 << endl; */

        // do BF
        /* cout << endl << how_many_things_in_knapsack << endl;

        for (int i = 0; i < how_many_things_in_knapsack; i++)
        {
            cout << "[" << knapsack[i].w << "|" << knapsack[i].c << "] ";
        }
        cout << endl; */
        // clear_knapsack();



        t5 = clock();
        PD_algorithm(n, b, B, P);
        t6 = clock();
        // cout << "Ile rzeczy " << how_many_things_in_knapsack << endl;



        /* find3 = t6 - t5;
        seconds3 = (double) find3 / CLOCKS_PER_SEC;
        cout << seconds3 << endl;
        plik2 << seconds3 << endl; */

        // do PD
        /* cout << "   ";
        for ( int i = 0; i <= b; i++) cout << setw(3) << i;
        cout << endl << endl;
        for( int i = 0; i <= n; i++)
        {
            cout << setw(3) << i;
            for ( int j = 0; j <= b; j++ ) cout << setw(3) << P[i][j];
            cout << endl;
        } */

        int licze = 0;
        // cout << endl << how_many_things_in_knapsack << endl;

        for (int i = 0; i < how_many_things_in_knapsack; i++)
        {
            // cout << "[" << knapsack[i].w << "|" << knapsack[i].c << "] ";
            licze += knapsack[i].c;
        }
        cout << endl;
        clear_knapsack();



        //czyszczenie
        delete []S;
        delete []weights;
        delete []costs;
        delete []P;
        if (B)
        {
            delete[] B;
            B = NULL;
        }
    }
    return 0;
}