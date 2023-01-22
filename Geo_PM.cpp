#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// functions with static
double e_sand(double qs);
void E_sand(int gradation, double qs, char water);
void E_clay(int gradation, double qs);
void R_all(int gradation, double qs, char water);
void cn_and_fn_sand(int gradation, double qs);
void cn_and_fn_clay(int gradation, double qs);
void p_sand(int gradation, double e);
void A_clay(double n, double v, double Xn);

// functions without static
void cn_fn_E_sand(int gradation, double e);
void sn_fn_E_clay(int gradation, double e, double Il);
void sn_fn_E_claym(int gradation, double e, double Il);
void sn_fn_E_clayl(int gradation, double e, double Il);
void R_1_4_n(int gradation, double e, double water);
void R_5_9_n(int gradation, double Il, double e);
double Wp(int gradation);
void nas(int gradation);

int main() {
    //setlocale(LC_ALL, "Rus");

    char geo_static;
    int gradation;

    cout << "Выберите вид зондирования? (s - статическое, d - динамическое, n - не проводилось, f - завершить): ";
    cin >> geo_static;

    cout << "Выберите тип грунта: ";
    cin >> gradation;

    while (geo_static == 's' || geo_static == 'd' || geo_static == 'n') {

        /*
        со статикой
        - по ТКП 45-5.01-15-2005(02250) (таблицы 6.1, 6.3, 6.6, 6.7, 6.8)
        */
        if (geo_static == 's') {

            double qs, alfa, n, v, Xn;
            char water;

            cout << "Введите удельное сопротивление грунта qs (по статике): ";
            cin >> qs;

            string name_gr[] = {"песок крупный", "песок средней крупности", "песок мелкий", "песок пылеватый",
                                "супесь (не моренная и не лессовидная)", "суглинок (не моренный и не лессовидный)", "суглинки и глины озерно-ледниковые",
                                "супесь (моренная)", "суглинок (моренный)", "супесь (лессовидная)", "суглинок (лессовидный)"};
            int num_n = gradation - 1;
            string name = name_gr[num_n];

            double e = e_sand(qs);

            if (gradation == 4)  {
                cout << "Песок водонасыщенный? (y - да, n - нет): ";
                cin >> water;
            }

            if (gradation >=1 && gradation <= 4) {
                cout << "Тип грунта: " << name << endl;
                cout << "e = " << e << endl;
                p_sand(gradation, e);
                cn_and_fn_sand(gradation, qs);
                E_sand(gradation, qs, water);
                R_all(gradation, qs, water);
            }

            else if (gradation >= 5 && gradation <= 11) {
                cout << "Кол-во монолитов 'n' (из sred): ";
                cin >> n;
                cout << "Коэффициент вариации 'v' (из sred): ";
                cin >> v;
                cout << "Нормативное значение 'Xn' (из sred): ";
                cin >> Xn;

                cout << "Тип грунта: " << name << endl;
                A_clay(n, v, Xn);
                cn_and_fn_clay(gradation,qs);
                E_clay(gradation, qs);
                R_all(gradation, qs, water);
            }
            else
                cout << "Введенный тип грунта не определен!" << endl;
        }

            /*
            без статики
            - по ТКП 45-5.01-67-2007 (таблицы Б1 - Б4, 5.5 - 5.7) - для обычных грунтов
            - по ТКП 45-3.03-112-2008 (таблицы 6.13, А1, А2) - для насыпных грунтов
            */
        else if (geo_static == 'n') {
            double e, Il, water;

            cout << "Введите значение коэффициента пористости e: ";
            cin >> e;

            string name_gr[] = {"песок крупный", "песок средней крупности", "песок мелкий", "песок пылеватый",
                                "супесь (не моренная и не лессовидная)", "суглинок (не моренный и не лессовидный)", "глина (не моренная и не лессовая)",
                                "супесь (моренная)", "суглинок (моренный)", "супесь (лессовидная)", "суглинок (лессовидный)"};
            int num_n = gradation - 1;
            string name = name_gr[num_n];

            if (gradation >=1 && gradation <= 4) {
                if (gradation == 3) {
                    cout << "Выберите степень водонасыщения (1 - маловлажные и влажные, 2 - водонасыщенные): ";
                    cin >> water;
                }
                if (gradation == 4) {
                    cout << "Выберите степень водонасыщения (1 - маловлажные, 2 - влажные, 3 - водонасыщенные): ";
                    cin >> water;
                }
                cout << "Тип грунта: " << name << endl;
                cn_fn_E_sand(gradation, e);
                R_1_4_n(gradation, e, water);
            }
            else if (gradation >= 5 && gradation <= 11) {

                cout << "Введите показатель текучести Il: ";
                cin >> Il;

                cout << "Тип грунта: " << name << endl;
                if (gradation >= 5 && gradation <= 7) {
                    sn_fn_E_clay(gradation, e, Il);
                }
                else if (gradation == 8 || gradation == 9) {
                    sn_fn_E_claym(gradation, e, Il);
                }
                else if (gradation == 10 || gradation == 11) {
                    sn_fn_E_clayl(gradation, e, Il);
                    cout << "R для лессовидных грунтов не определено" << endl;
                }
                R_5_9_n(gradation, Il, e);
            }
            else if (gradation == 12) {
                nas(gradation);
            }
            else
                cout << "Введенный тип грунта не определен!" << endl;
        }

        else {
            cout << "Выберите правильно вид зондирования (s - статическое, d - динамическое, n - не проводилось)" << endl;
        }

        cout << "\n";
        cout << "Выберите тип грунта: ";
        cin >> gradation;

        if (gradation == 0) {
            cout << "\n";
            cout << "Выберите вид зондирования? (s - статическое, d - динамическое, n - не проводилось, f - завершить): ";
            cin >> geo_static;
        }
    }

    if (geo_static == 'f') {
        cout << "\n";
        cout << "Спасибо, что воспользовались моей программой!" << endl << "\n" << "Product by A.Deshkovets" << endl;
    }

    return 0;
}

// FUNCTIONS WITH STATIC
/* Коэффициент пористости песчаных грунтов "e", независимо от генезиса, гранулометрического состава и влажности (таблица 6.1) */
double e_sand(double qs) {
    int min = 0, max = 0;
    double e = 0, e_min = 0, e_max = 0, q_min = 0, q_max = 0;
    const int ROWS = 2;
    const int COLS = 9;
    double e_sand[ROWS][COLS] = {
            {1, 2, 3, 5, 10, 15, 20, 30, 50},
            {0.82, 0.74, 0.70, 0.64, 0.58, 0.53, 0.50, 0.46, 0.41}
    };

    if (qs > e_sand[0][8]) qs = e_sand[0][8];
    else if (qs < e_sand[0][0]) qs = e_sand[0][0];

    for (int i = 0; i < COLS; i++) {
        if (qs >= e_sand[0][i]) {
            min = i;
            max = i + 1;
        }
    }

    q_min = e_sand[0][min];
    q_max = e_sand[0][max];
    e_min = e_sand[1][max];
    e_max = e_sand[1][min];

    e = e_min + (e_max - e_min) / (q_max - q_min) * (q_max - qs);

    return e;
}

/* Значение модуля деформации "Е" (МПа) для песков (таблица 6.7) */
void E_sand(int gradation, double qs, char water) {

    double E = 0, E_min = 0, E_max = 0, q_min = 0, q_max = 0;
    int min = 0, max = 0, number = 0;
    const int ROWS = 5; //количество строк в массиве
    const int COLS = 9; //количество колонок в массиве

    double E_sand [ROWS][COLS] = {
            {1, 2, 4, 6, 8, 10, 12, 15, 20}, // q
            {10, 15, 21, 25, 32, 38, 45, 50, 60}, // gradation = 1, gradation = 2
            {8, 12, 18, 22, 26, 30, 36, 42, 50}, // gradation = 3
            {7, 10, 14, 18, 21, 25, 30, 35, 40}, // gradation = 4, water = n
            {6, 8, 10, 14, 18, 21, 25, 30, 35} // gradation = 4, water = y
    };

    if (qs > E_sand[0][8]) qs = E_sand[0][8];
    else if (qs < E_sand[0][0]) qs = E_sand[0][0];

    for (int i = 0; i < COLS; i++) {
        if (qs >= E_sand[0][i]) {
            min = i;
            max = i + 1;
        }
    }

    if (gradation == 1 || gradation == 2) number = 1;
    else if (gradation == 3) number = 2;
    else if (gradation == 4 && water == 'n') number = 3;
    else if (gradation == 4 && water == 'y') number = 4;

    q_min = E_sand[0][min];
    q_max = E_sand[0][max];
    E_min = E_sand[number][min];
    E_max = E_sand[number][max];

    E = E_max - (E_max - E_min) / (q_max - q_min) * (q_max - qs);

    cout << "E (MПа) = " << E << endl;
}

/* Значение модуля деформации "E" (МПа) для глинистых грунтов */
void E_clay(int gradation, double qs) {

    double E = 0;

    if (gradation == 5 || gradation == 10) {
        E = 59.685 * qs / 10.4;
    }
    else if (gradation == 6 || gradation == 11) {
        E = 58.4668 * qs / 9.6;
    }
    else if (gradation == 7) {
        E = 60.0996 * qs / 8.8;
    }
    else if (gradation == 8) {
        E = 54.26 * qs / 10.4;
    }
    else if (gradation == 9) {
        E = 52.3124 * qs / 9.6;
    }

    cout << "E = " << E << endl;
}

/* Значение условного расчетного сопротивления грунтов "R" (МПа) (таблица 6.8) */
void R_all(int gradation, double qs, char water) {
    double R = 0, R_min = 0, R_max = 0, q_min = 0, q_max = 0;
    int min = 0, max = 0, number = 0;
    const int ROWS = 7; //количество строк в массиве
    const int COLS = 7; //количество колонок в массиве

    double R_all[ROWS][COLS] = {
            {1, 2, 3, 5, 8, 10, 15},
            {0.13, 0.21, 0.29, 0.45, 0.60, 0.60, 0.60},
            {0.10, 0.18, 0.25, 0.35, 0.45, 0.50, 0.60},
            {0.0, 0.15, 0.20, 0.30, 0.40, 0.50, 0.70},
            {0.12, 0.15, 0.25, 0.30, 0.35, 0.40, 0.50},
            {0.10, 0.12, 0.20, 0.25, 0.30, 0.35, 0.35},
            {0.08, 0.10, 0.15, 0.20, 0.25, 0.30, 0.30}
    };

    if (qs > R_all[0][6]) qs = R_all[0][6];
    else if (qs < R_all[0][0]) qs = R_all[0][0];

    for (int i = 0; i < COLS; i++) {
        if (qs >= R_all[0][i]) {
            min = i;
            max = i + 1;
        }
    }

    q_min = R_all[0][min];
    q_max = R_all[0][max];

    if (gradation == 1 || gradation == 2) number = 3;
    else if (gradation == 3) number = 4;
    else if (gradation == 4 && water == 'n') number = 5;
    else if (gradation == 4 && water == 'y') number = 6;
    else if (gradation == 5 || gradation == 6 || gradation == 7 || gradation == 10 || gradation == 11) number = 2;
    else if (gradation == 8 || gradation == 9) number = 1;

    R_min = R_all[number][min];
    R_max = R_all[number][max];

    R = R_max - (R_max - R_min) / (q_max - q_min) * (q_max - qs);

    cout << "R (MПа) = " << R << endl;
}

/* Значение характеристик песчаных грунтов при статике (таблица 6.3) */
void cn_and_fn_sand(int gradation, double qs) {
    double cn = 0, fn = 0,  cn_min = 0, cn_max = 0, fn_min = 0, fn_max = 0, q_min = 0, q_max = 0;
    int min = 0, max = 0, number = 0;
    const int ROWS = 4; //количество строк в массиве
    const int COLS = 10; //количество колонок в массиве
    double q[] = {1, 2, 3, 5, 8, 10, 15, 20, 30, 31};
    double c_sand[ROWS][COLS] = {
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.001, 0.001, 0.001, 0.002, 0.002},
            {0.0, 0.0, 0.0, 0.001, 0.001, 0.002, 0.002, 0.002, 0.003, 0.003},
            {0.0, 0.0, 0.001, 0.002, 0.002, 0.003, 0.004, 0.004, 0.005, 0.006},
            {0.0, 0.002, 0.003, 0.004, 0.004, 0.005, 0.006, 0.006, 0.007, 0.008}
    };
    double f_sand[ROWS][COLS] = {
            {30, 32, 34, 36, 38, 39, 40, 41, 42, 43},
            {28, 30, 32, 35, 36, 37, 38, 38, 39, 40},
            {26, 29, 30, 32, 34, 35, 36, 36, 37, 38},
            {24, 26, 28, 30, 32, 33, 34, 34, 35, 36}
    };

    if (qs > q[9]) qs = q[9];
    else if (qs < q[0]) qs = q[0];

    for (int i = 0; i < COLS; i++) {
        if (qs >= q[i]) {
            min = i;
            max = i + 1;
        }
    }

    if (gradation == 1) number = 0;
    else if (gradation == 2) number = 1;
    else if (gradation == 3) number = 2;
    else if (gradation == 4) number = 3;

    q_min = q[min];
    q_max = q[max];
    cn_min = c_sand[number][min];
    cn_max = c_sand[number][max];
    fn_min = f_sand[number][min];
    fn_max = f_sand[number][max];

    cn = cn_min + (cn_max - cn_min) / (q_max - q_min) * (qs - q_min); // cn = cn_min + (cn_max - cn_min) / (q_max - q_min) * (qs - cn_min)
    fn = fn_max - (fn_max - fn_min) / (q_max - q_min) * (q_max - qs); // обе формулы верны

    double cII = cn;
    double cI = cn / 1.5;
    double fII = fn;
    double fI = fn / 1.1;

    cout << "cn (МПа) = " << cn << endl;
    cout << "cII / cI = " << cII << " / " << cI << endl;
    cout << "fn (град) = " << fn << endl;
    cout << "fII / fI = " << fII << " / " << fI << endl;
}

/* Значение характеристик глинистых грунтов при статике (таблица 6.6) */
void cn_and_fn_clay(int gradation, double qs) {
    double cn = 0, fn = 0,  cn_min = 0, cn_max = 0, fn_min = 0, fn_max = 0, q_min = 0, q_max = 0;
    int min = 0, max = 0, cn_number = 0, fn_number = 0;
    const int ROWS = 15; //количество строк в массиве
    const int COLS = 7; //количество колонок в массиве

    double cf_clay[ROWS][COLS] = {
            {1, 2, 3, 5, 8, 10, 12},
            {0.011, 0.013, 0.015, 0.017, 0.019, 0.021, 0.024},
            {18, 21, 24, 27, 29, 29, 30},
            {0.016, 0.023, 0.025, 0.028, 0.035, 0.039, 0.047},
            {16, 18, 20, 22, 24, 25, 26},
            {0.036, 0.043, 0.056, 0.066, 0.087, 0.102, 0.130},
            {14, 14, 13, 12, 11, 10, 9},
            {0.023, 0.027, 0.031, 0.036, 0.040, 0.042, 0.048},
            {26, 27, 27, 28, 29, 30, 31},
            {0.030, 0.036, 0.040, 0.045, 0.049, 0.052, 0.056},
            {25, 26, 26, 27, 28, 29, 29},
            {0.018, 0.022, 0.026, 0.030, 0.032, 0.034, 0.036},
            {22, 26, 27, 27, 28, 28, 29},
            {0.025, 0.030, 0.034, 0.036, 0.041, 0.046, 0.052},
            {21, 23, 24, 25, 26, 27, 28}
    };

    if (qs > cf_clay[0][6]) qs = cf_clay[0][6];
    else if (qs < cf_clay[0][0]) qs = cf_clay[0][0];

    for (int i = 0; i < COLS; i++) {
        if (qs >= cf_clay[0][i]) {
            min = i;
            max = i + 1;
        }
    }

    if (gradation == 5) {
        cn_number = 1;
        fn_number = 2;
    }
    else if (gradation == 6) {
        cn_number = 3;
        fn_number = 4;
    }
    else if (gradation == 7) {
        cn_number = 5;
        fn_number = 6;
    }
    else if (gradation == 8) {
        cn_number = 7;
        fn_number = 8;
    }
    else if (gradation == 9) {
        cn_number = 9;
        fn_number = 10;
    }
    else if (gradation == 10) {
        cn_number = 11;
        fn_number = 12;
    }
    else if (gradation == 11) {
        cn_number = 13;
        fn_number = 14;
    }

    q_min = cf_clay[0][min];
    q_max = cf_clay[0][max];
    cn_min = cf_clay[cn_number][min];
    cn_max = cf_clay[cn_number][max];
    fn_min = cf_clay[fn_number][min];
    fn_max = cf_clay[fn_number][max];

    cn = cn_max - (cn_max - cn_min) / (q_max - q_min) * (q_max - qs); // cn = cn_min + (cn_max - cn_min) / (q_max - q_min) * (qs - cn_min)
    fn = fn_max - (fn_max - fn_min) / (q_max - q_min) * (q_max - qs); // обе формулы верны

    double cII = cn;
    double cI = cn /1.5;
    double fII = fn;
    double fI = fn / 1.15;

    cout << "cn (МПа)= " << cn << endl;
    cout << "cII / cI = " << cII << " / " << cI << endl;
    cout << "fn (град) = " << fn << endl;
    cout << "fII / fI = " << fII << " / " << fI << endl;
}

/* Расчет плотности и удельного веса песчаного грунта */
void p_sand(int gradation, double e) {
    /* индекс m - маловлажное состояние,
          w - водонасыщенное состояние,
          wz - с учетом взвешивающего действия воды
    */

    double W_m = 0, W_w = 0, ps = 0, pd = 0, n = 0, Sr = 0, p_m = 0, p_w = 0, p_wz = 0;

    if (gradation == 1) {
        W_m = 5.0;
        W_w = 21.0;
        ps = 2.65;
    }
    else if (gradation == 2) {
        W_m = 5.0;
        W_w = 22.5;
        ps = 2.65;
    }
    else if (gradation == 3) {
        W_m = 5.0;
        W_w = 20.0;
        ps = 2.65;
    }
    if (gradation == 4) {
        W_m = 5.0;
        W_w = 25.6;
        ps = 2.66;
    }

    p_m =  ps * (1 + W_m / 100) / (1 + e);
    pd = p_m / (1 + W_m * 0.01);
    n = ((ps - pd) / ps) * 100;
    p_w = pd + (n / 100);
    Sr = (ps * W_m / e) / 100;
    p_wz = (ps - 1) * (1 - n / 100);

    // округление
    p_m = round(p_m * 100) / 100;
    p_w = round(p_w * 100) / 100;
    p_wz = round(p_wz * 100) / 100;

    double gamma_m = p_m * 9.81;
    double gamma_wz = p_wz * 9.81;

    cout << "p = " << p_m << " / " << p_w << endl;
    cout << "gamma = " << gamma_m << " / " << gamma_wz << endl;
}

/* Расчет удельного веса глинистого грунта */
void A_clay(double n, double v, double Xn){
    const int ROWS = 3;
    const int COLS = 22;

    double ta_table[ROWS][COLS] = {
            {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25, 30, 40, 60}, // число степеней свободы K
            {1.25, 1.19, 1.16, 1.13, 1.12, 1.11, 1.10, 1.10, 1.09, 1.08, 1.08, 1.08, 1.07, 1.07, 1.07, 1.07, 1.07, 1.06, 1.06, 1.05, 1.05, 1.05}, // alfa = 0.85
            {2.35, 2.13, 2.01, 1.94, 1.90, 1.86, 1.83, 1.81, 1.80, 1.78, 1.77, 1.76, 1.75, 1.75, 1.74, 1.73, 1.73, 1.72, 1.71, 1.70, 1.68, 1.67} // alfa = 0.95
    };

    /*
    ГОСТ 20522-2012, таблица Е2
    n - кол-во монолитов (из sred),
    v - коэффициент вариации (из sred),
    An - нормативное значение (из sred)
    px - какой-то коэффициент ???
    Kr - какой-то коэффициент ???
    */

    int num = 0;
    for (int i = 0; i < COLS; i++) {
        if ((n - 1) == ta_table[0][i]) {
            num = i;
        }
    }

    double px2 = (ta_table[1][num] * v) / sqrt(n);
    double px1 = (ta_table[2][num] * v) / sqrt(n);
    double Kr1 = 1 + px1;
    double Kr2 = 1 + px2;
    double A1 = Xn / Kr1;
    double A2 = Xn / Kr2;
    double gamma = Xn * 9.81;
    double gamma_II = A2 * 9.81;
    double gamma_I = A1 * 9.81;

    cout << "gamma = " << gamma << endl;
    cout << "gamma_II = " << gamma_II << endl;
    cout << "gamma_I = " << gamma_I << endl;
}

// FUNCTIONS WITHOUT STATIC
/* расчеты для песчаных грунтов по таблице Б1 ТКП 45-5.01-67-2007 */
void cn_fn_E_sand(int gradation, double e) {
    double cn = 0, fn = 0, E = 0,  cn_min = 0, cn_max = 0, fn_min = 0, fn_max = 0, E_min = 0, E_max = 0, e_min = 0, e_max = 0;
    int min = 0, max = 0, number = 0;
    const int ROWS = 4; //количество строк в массиве
    const int COLS = 4; //количество колонок в массиве

    double et[] = {0.45, 0.55, 0.65, 0.75};
    double c_sand[ROWS][COLS] = {
            {2, 1, 0, 0},
            {3, 2, 1, 0},
            {6, 4, 2, 0},
            {8, 6, 4, 2}
    };
    double f_sand[ROWS][COLS] = {
            {43, 40, 38, 35},
            {40, 38, 35, 33},
            {38, 36, 32, 28},
            {36, 34, 30, 26}
    };
    double E_sand[ROWS][COLS] = {
            {50, 40, 30, 15},
            {45, 35, 25, 13},
            {40, 30, 20, 12},
            {35, 25, 18, 11}
    };

    if (e > et[3]) e = et[3];
    else if (e < et[0]) e = et[0];

    for (int i = 0; i < COLS; i++) {
        if (e >= et[i]) {
            min = i;
            max = i + 1;
        }
    }

    if (gradation == 1) number = 0;
    else if (gradation == 2) number = 1;
    else if (gradation == 3) number = 2;
    else if (gradation == 4) number = 3;

    e_min = et[min];
    e_max = et[max];
    cn_min = c_sand[number][min];
    cn_max = c_sand[number][max];
    fn_min = f_sand[number][min];
    fn_max = f_sand[number][max];
    E_min = E_sand[number][min];
    E_max = E_sand[number][max];

    cn = cn_max - (cn_max - cn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    fn = fn_max - (fn_max - fn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    E = E_max - (E_max - E_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;

    cout << "cn = " << cn / 1000 << "\t" << "МПа" << endl;
    cout << "fn = " << fn << "\t" << "град" << endl;
    cout << "E = " << E << "\t" << "МПа" << endl;
}

/* расчеты для глинистых грунтов (не моренного и не лессового происхождения) по таблице Б2 ТКП 45-5.01-67-2007 */
void sn_fn_E_clay(int gradation, double e, double Il) {
    double cn = 0, fn = 0, E = 0,  cn_min = 0, cn_max = 0, fn_min = 0, fn_max = 0, E_min = 0, E_max = 0, e_min = 0, e_max = 0,
            Il_min = 0, Il_max = 0;
    int min = 0, max = 0, number = 0;
    const int ROWS = 7; //количество строк в массиве
    const int COLS = 7; //количество колонок в массиве

    double et[] = {0.45, 0.55, 0.65, 0.75, 0.85, 0.95, 1.05};
    int c_clay[ROWS][COLS] = {
            {21, 17, 15, 13, 0, 0, 0},
            {19, 15, 13, 11, 9, 0, 0},
            {47, 37, 31, 25, 22, 19, 0},
            {39, 34, 28, 23, 18, 15, 0},
            {0, 0, 25, 20, 16, 14, 12},
            {0, 81, 68, 54, 47, 41, 36},
            {0, 0, 57, 50, 43, 37, 32}
    };
    int f_clay[ROWS][COLS] = {
            {30, 29, 27, 24, 0, 0, 0},
            {28, 26, 24, 21, 18, 0, 0},
            {26, 25, 24, 23, 22, 20, 0},
            {24, 23, 22, 21, 19, 17, 0},
            {0, 0, 19, 18, 16, 14, 12},
            {0, 21, 20, 19, 18, 16, 14},
            {0, 0, 18, 17, 16, 14, 11}
    };
    int E_clay[ROWS][COLS] = {
            {32, 24, 16, 10, 7, 0, 0},
            {31, 23, 15, 9, 6, 0, 0},
            {34, 27, 22, 17, 14, 11, 0},
            {32, 25, 19, 14, 11, 8, 0},
            {0, 0, 17, 12, 8, 6, 5},
            {0, 28, 24, 21, 18, 15, 12},
            {0, 0, 21, 18, 15, 12, 9}
    };

    if (e > et[6]) e = et[6];
    else if (e < et[0]) e = et[0];

    if (Il < 0) Il = 0.1;
    else if (Il > 0.75) Il = 0.75;

    for (int i = 0; i < COLS; i++) {
        if (e >= et[i]) {
            min = i;
            max = i + 1;
        }
    }

    if (gradation == 5) {
        if (Il >= 0 && Il <= 0.25) number = 0;
        else if (Il > 0.25 && Il <= 0.75) number = 1;
    }
    else if (gradation == 6) {
        if (Il >= 0 && Il <= 0.25) number = 2;
        else if (Il > 0.25 && Il <= 0.5) number = 3;
        else if (Il > 0.5 && Il <= 0.75) number = 4;
    }
    else if (gradation == 7) {
        if (Il < 0) Il = 0.1;
        else if (Il > 0.5) Il = 0.5;

        if (Il >= 0 && Il <= 0.25) number = 5;
        else if (Il > 0.25 && Il <= 0.5) number = 6;
    }

    e_min = et[min];
    e_max = et[max];
    cn_min = c_clay[number][min];
    cn_max = c_clay[number][max];
    fn_min = f_clay[number][min];
    fn_max = f_clay[number][max];
    E_min = E_clay[number][min];
    E_max = E_clay[number][max];

    cn = cn_max - (cn_max - cn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    fn = fn_max - (fn_max - fn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    E = E_max - (E_max - E_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;

    cout << "cn = " << cn / 1000 << "\t" << "МПа" << endl;
    cout << "fn = " << fn << "\t" << "град" << endl;
    cout << "E = " << E << "\t" << "МПа" << endl;
}

/* расчеты для глинистых грунтов (моренных) по таблице Б3 ТКП 45-5.01-67-2007 */
void sn_fn_E_claym(int gradation, double e, double Il) {
    double cn = 0, fn = 0, E = 0,  cn_min = 0, cn_max = 0, fn_min = 0, fn_max = 0, E_min = 0, E_max = 0, e_min = 0, e_max = 0,
            Il_min = 0, Il_max = 0;
    int min = 0, max = 0, number = 0;
    const int ROWS = 8; //количество строк в массиве
    const int COLS = 4; //количество колонок в массиве

    double et[] = {0.30, 0.35, 0.45, 0.50};
    int c_clay[ROWS][COLS] = {
            {44, 40, 31, 0},
            {40, 37, 29, 0},
            {33, 30, 21, 18},
            {0, 0, 18, 15},
            {50, 48, 45, 0},
            {46, 43, 36, 0},
            {0, 36, 31, 27},
            {0, 0, 27, 25}
    };
    int f_clay[ROWS][COLS] = {
            {31, 30, 27, 0},
            {30, 29, 25, 0},
            {29, 28, 24, 22},
            {0, 0, 22, 21},
            {29, 28, 25, 0},
            {28, 27, 24, 0},
            {0, 26, 23, 21},
            {0, 0, 21, 20}
    };
    int E_clay[ROWS][COLS] = {
            {40, 34, 30, 0},
            {32, 26, 23, 0},
            {26, 20, 17, 14},
            {0, 0, 14, 10},
            {48, 39, 34, 0},
            {40, 31, 27, 0},
            {0, 24, 20, 15},
            {0, 0, 15, 8}
    };

    if (e > et[3]) e = et[3];
    else if (e < et[0]) e = et[0];

    if (Il < -0.25) Il = -0.25;
    else if (Il > 0.75) Il = 0.75;

    for (int i = 0; i < COLS; i++) {
        if (e >= et[i]) {
            min = i;
            max = i + 1;
        }
    }

    if (gradation == 8) {
        if (Il >= -0.25 && Il <= 0) number = 0;
        else if (Il > 0 && Il <= 0.25) number = 1;
        else if (Il > 0.25 && Il <= 0.5) number = 2;
        else if (Il > 0.5 && Il <= 0.75) number = 3;
    }
    else if (gradation == 9) {
        if (Il >= -0.25 && Il <= 0) number = 4;
        else if (Il > 0 && Il <= 0.25) number = 5;
        else if (Il > 0.25 && Il <= 0.5) number = 6;
        else if (Il > 0.5 && Il <= 0.75) number = 7;
    }

    e_min = et[min];
    e_max = et[max];
    cn_min = c_clay[number][min];
    cn_max = c_clay[number][max];
    fn_min = f_clay[number][min];
    fn_max = f_clay[number][max];
    E_min = E_clay[number][min];
    E_max = E_clay[number][max];

    cn = cn_max - (cn_max - cn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    fn = fn_max - (fn_max - fn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    E = E_max - (E_max - E_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;

    cout << "cn = " << cn / 1000 << "\t" << "МПа" << endl;
    cout << "fn = " << fn << "\t" << "град" << endl;
    cout << "E = " << E << "\t" << "МПа" << endl;
}

/* расчеты для глинистых грунтов (лессовидных) по таблице Б4 ТКП 45-5.01-67-2007 */
void sn_fn_E_clayl(int gradation, double e, double Il) {
    double cn = 0, fn = 0, E = 0,  cn_min = 0, cn_max = 0, fn_min = 0, fn_max = 0, E_min = 0, E_max = 0, e_min = 0, e_max = 0,
            Il_min = 0, Il_max = 0;
    int min = 0, max = 0, number = 0;
    const int ROWS = 8; //количество строк в массиве
    const int COLS = 4; //количество колонок в массиве

    double et[] = {0.55, 0.65, 0.75, 0.85};
    int c_clay[ROWS][COLS] = {
            {36, 28, 24, 22},
            {28, 24, 22, 20},
            {26, 22, 20, 18},
            {24, 20, 18, 15},
            {41, 36, 31, 27},
            {36, 31, 27, 23},
            {31, 27, 23, 20},
            {27, 22, 20, 18}
    };
    int f_clay[ROWS][COLS] = {
            {30, 28, 26, 24},
            {29, 27, 25, 23},
            {28, 26, 24, 22},
            {27, 25, 23, 21},
            {26, 24, 23, 22},
            {26, 24, 22, 21},
            {25, 23, 21, 20},
            {24, 22, 20, 18}
    };
    int E_clay[ROWS][COLS] = {
            {22, 18, 12, 10},
            {18, 12, 10, 8},
            {12, 10, 8, 7},
            {10, 8, 6, 5},
            {25, 20, 15, 12},
            {20, 15, 12, 10},
            {15, 12, 9, 7},
            {12, 9, 7, 5}
    };

    if (e > et[3]) e = et[3];
    else if (e < et[0]) e = et[0];

    if (Il < 0) Il = 0;
    else if (Il > 0.75) Il = 0.75;

    for (int i = 0; i < COLS; i++) {
        if (e >= et[i]) {
            min = i;
            max = i + 1;
        }
    }

    if (gradation == 10) {
        if (Il == 0) number = 0;
        else if (Il > 0 && Il <= 0.25) number = 1;
        else if (Il > 0.25 && Il <= 0.5) number = 2;
        else if (Il > 0.5 && Il <= 0.75) number = 3;
    }
    else if (gradation == 11) {
        if (Il == 0) number = 4;
        else if (Il > 0 && Il <= 0.25) number = 5;
        else if (Il > 0.25 && Il <= 0.5) number = 6;
        else if (Il > 0.5 && Il <= 0.75) number = 7;
    }

    e_min = et[min];
    e_max = et[max];
    cn_min = c_clay[number][min];
    cn_max = c_clay[number][max];
    fn_min = f_clay[number][min];
    fn_max = f_clay[number][max];
    E_min = E_clay[number][min];
    E_max = E_clay[number][max];

    cn = cn_max - (cn_max - cn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    fn = fn_max - (fn_max - fn_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
    E = E_max - (E_max - E_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;

    cout << "cn = " << cn / 1000 << "\t" << "МПа" << endl;
    cout << "fn = " << fn << "\t" << "град" << endl;
    cout << "E = " << E << "\t" << "МПа" << endl;
}

/* Значение условного расчетного сопротивления грунтов "R" (МПа) по таблице 5.5 ТКП 45-5.01-67-2007 */
void R_1_4_n(int gradation, double e, double water) {

    double R = 0;

    const int ROWS = 2;
    const int COLS = 7;

    double R_1_4[ROWS][COLS] = {
            {600, 500, 400, 300, 300, 250, 200},
            {500, 400, 300, 250, 250, 150, 100}
    };

    if (e >= 0.45 && e <= 0.54) {
        if (gradation == 1) R = R_1_4[0][0];
        else if (gradation == 2) R = R_1_4[0][1];
        else if (gradation == 3) {
            if (water == 1) R = R_1_4[0][2];
            else if (water == 2) R = R_1_4[0][3];
        }
        else if (gradation == 4) {
            if (water == 1) R = R_1_4[0][4];
            else if (water == 2) R = R_1_4[0][5];
            else if (water == 3) R = R_1_4[0][6];
        }
    }
    else if (e >= 0.55 && e <= 0.75) {
        if (gradation == 1) R = R_1_4[1][0];
        else if (gradation == 2) R = R_1_4[1][1];
        else if (gradation == 3) {
            if (water == 1) R = R_1_4[1][2];
            else if (water == 2) R = R_1_4[1][3];
        }
        else if (gradation == 4) {
            if (water == 1) R = R_1_4[1][4];
            else if (water == 2) R = R_1_4[1][5];
            else if (water == 3) R = R_1_4[1][6];
        }
    }

    cout << "R = " << R / 1000 << "\t" << "МПа" << endl;
}

/* Значение условного расчетного сопротивления грунтов "R" (МПа) по таблице 5.6 - 5.7 ТКП 45-5.01-67-2007 */
void R_5_9_n(int gradation, double Il, double e) {
    double R = 0, R_min = 0, R_max = 0, Il_min = 0, Il_max = 0, e_min = 0, e_max = 0;
    double R1_min = 0, R1_max = 0, R2_min = 0, R2_max = 0, R1 = 0, R2 = 0;
    int min = 0, max = 0, Iln_min = 0, Iln_max = 0, num_t = 0;

    if (gradation >=5 && gradation <= 7) {
        const int ROWS = 4;
        const int COLS = 3;

        double Ilt[COLS] = {0, 0.5, 0.75};
        double et1[2] = {0.5, 0.7};
        double et2[3] = {0.5, 0.7, 0.85};
        double et3[4] = {0.5, 0.6, 0.8, 1.0};

        if (gradation == 5) {
            for (int j = 0; j < 2; j++) {
                if (e >= et1[j]) {
                    min = j;
                    max = j + 1;
                }
            }
            e_min = et1[min];
            e_max = et1[max];
        }
        else if (gradation == 6) {
            for (int j = 0; j < 3; j++) {
                if (e >= et2[j]) {
                    min = j;
                    max = j + 1;
                }
            }
            e_min = et2[min];
            e_max = et2[max];
        }
        else if (gradation == 7) {
            for (int j = 0; j < 4; j++) {
                if (e >= et3[j]) {
                    min = j;
                    max = j + 1;
                }
            }
            e_min = et3[min];
            e_max = et3[max];
        }

        double Rt1[ROWS][COLS] = {
                {400, 300, 250},
                {300, 250, 200}
        };
        double Rt2[ROWS][COLS] = {
                {400, 350, 300},
                {350, 300, 200},
                {250, 200, 150}
        };
        double Rt3[ROWS][COLS] = {
                {600, 500, 400},
                {500, 400, 300},
                {300, 250, 200},
                {250, 200, 150}
        };

        if (Il > Ilt[2]) Il = Ilt[2];
        else if (Il < Ilt[0]) Il = Ilt[0];

        for (int i = 0; i < COLS; i++) {
            if (Il >= Ilt[i]) {
                Iln_min = i;
                Iln_max = i + 1;
            }
        }

        Il_min = Ilt[Iln_min];
        Il_max = Ilt[Iln_max];

        if (e == et1[min] || e == et2[min] || e == et3[min] || Il == Ilt[Iln_min]) {
            if (e == et1[min] || e == et2[min] || e == et3[min]){
                if (gradation == 5) {
                    R_max = Rt1[min][Iln_min];
                    R_min = Rt1[min][Iln_max];
                }
                else if (gradation == 6) {
                    R_max = Rt2[min][Iln_min];
                    R_min = Rt2[min][Iln_max];
                }
                else if (gradation == 7) {
                    R_max = Rt2[min][Iln_min];
                    R_min = Rt2[min][Iln_max];
                }
                R = R_min + (R_max - R_min) / ((Il_max - Il_min) * 100) * (Il_max - Il) * 100;
            }
            else if (Il == Ilt[Iln_min]) {
                if (gradation == 5) {
                    R_max = Rt1[min][Iln_min];
                    R_min = Rt1[max][Iln_min];
                }
                else if (gradation == 6) {
                    R_max = Rt2[min][Iln_min];
                    R_min = Rt2[max][Iln_min];
                }
                else if (gradation == 7) {
                    R_max = Rt3[min][Iln_min];
                    R_min = Rt3[max][Iln_min];
                }
                R = R_min + (R_max - R_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
            }
        }
        else if (e != et1[min] && e != et2[min] && e != et3[min] && Il != Ilt[Iln_min]) {
            if (gradation == 5) {
                R1_max = Rt1[min][Iln_min];
                R1_min = Rt1[max][Iln_min];
                R2_max = Rt1[min][Iln_max];
                R2_min = Rt1[max][Iln_max];
            }
            else if (gradation == 6) {
                R1_max = Rt2[min][Iln_min];
                R1_min = Rt2[max][Iln_min];
                R2_max = Rt2[min][Iln_max];
                R2_min = Rt2[max][Iln_max];
            }
            else if (gradation == 7) {
                R1_max = Rt3[min][Iln_min];
                R1_min = Rt3[max][Iln_min];
                R2_max = Rt3[min][Iln_max];
                R2_min = Rt3[max][Iln_max];
            }

            R1 = R1_min + (R1_max - R1_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
            R2 = R2_min + (R2_max - R2_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
            R = R2 - (R2 - R1) / ((Il_max - Il_min) * 100) * (Il_max - Il) * 100;
        }
    }
    else if (gradation == 8 || gradation == 9) {
        const int TABLES = 2;
        const int ROWS = 3;
        const int COLS = 4;

        double Ilt[COLS] = {0, 0.25, 0.5, 0.75};
        double et[ROWS] = {0.3, 0.35, 0.45};

        double Rt[TABLES][ROWS][COLS] = {
                {
                        {600, 500, 0, 0},
                        {500, 400, 300, 0},
                        {400, 300, 200, 150}
                },
                {
                        {600, 540, 400, 0},
                        {500, 450, 300, 0},
                        {420, 350, 250, 200}
                }
        };

        if (Il > Ilt[3]) Il = Ilt[3];
        else if (Il < Ilt[0]) Il = Ilt[0];

        for (int i = 0; i < COLS; i++) {
            if (Il >= Ilt[i]) {
                Iln_min = i;
                Iln_max = i + 1;
            }
        }
        for (int j = 0; j < ROWS; j++) {
            if (e >= et[j]) {
                min = j;
                max = j + 1;
            }
        }

        Il_min = Ilt[Iln_min];
        Il_max = Ilt[Iln_max];
        e_min = et[min];
        e_max = et[max];

        if (gradation == 8) num_t = 0;
        else if (gradation == 9) num_t = 1;

        if (e == et[min] || Il == Ilt[Iln_min]) {
            if (e == et[min]){
                R_max = Rt[num_t][min][Iln_min];
                R_min = Rt[num_t][min][Iln_max];
                R = R_min + (R_max - R_min) / ((Il_max - Il_min) * 100) * (Il_max - Il) * 100;
            }
            else if (Il == Ilt[Iln_min]) {
                R_max = Rt[num_t][min][Iln_min];
                R_min = Rt[num_t][max][Iln_min];
                R = R_min + (R_max - R_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
            }
        }
        else if (e != et[min] && Il != Ilt[Iln_min]) {
            R1_max = Rt[num_t][min][Iln_min];
            R1_min = Rt[num_t][max][Iln_min];
            R2_max = Rt[num_t][min][Iln_max];
            R2_min = Rt[num_t][max][Iln_max];
            R1 = R1_min + (R1_max - R1_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
            R2 = R2_min + (R2_max - R2_min) / ((e_max - e_min) * 100) * (e_max - e) * 100;
            R = R2 - (R2 - R1) / ((Il_max - Il_min) * 100) * (Il_max - Il) * 100;
        }
    }

    /*cout << "e_min = " << e_min << "\t" << "МПа" << endl;
    cout << "e_max = " << e_max << "\t" << "МПа" << endl;
    cout << "Il_min = " << Il_min << "\t" << "МПа" << endl;
    cout << "Il_max = " << Il_max << "\t" << "МПа" << endl;
    cout << "R_min = " << R_min << "\t" << "МПа" << endl;
    cout << "R_max = " << R_max << "\t" << "МПа" << endl;
    cout << "R1_min = " << R1_min << "\t" << "МПа" << endl;
    cout << "R1_max = " << R1_max << "\t" << "МПа" << endl;
    cout << "R2_min = " << R2_min << "\t" << "МПа" << endl;
    cout << "R2_max = " << R2_max << "\t" << "МПа" << endl;
    cout << "R1 = " << R1 << "\t" << "МПа" << endl;
    cout << "R2 = " << R2 << "\t" << "МПа" << endl;*/

    cout << "R = " << R / 1000 << "\t" << "МПа" << endl;
}

/* Насыпные грунты ТКП 45-3.03-112-2008 */
// получение расчетного значения влажности грунта Wp, % по таблице А2
// Коэффициент нормированного отклонения t по таблице 6.13
double Wp(int gradation) {
    double C = 0, f = 0, Ey = 0,  C_min = 0, C_max = 0, f_min = 0, f_max = 0, Ey_min = 0, Ey_max = 0, t = 0;
    int min = 0, max = 0, number = 0;

    if (gradation > 4) {
        double kn;
        cout << "Введите коэффициент надежности kn: ";
        cin >> kn;

        // t в зависимости от kn (по таблице 6.13 ТКП 45-3.03-112-2008)
        if (kn == 0.85) t = 1.06;
        else if (kn == 0.90) t = 1.32;
        else if (kn == 0.95) t = 1.71;
        else if (kn == 0.98) t = 2.19;
    }

    int dkr; // дорожно-климатический район (1, 2, 3)
    cout << "Выберите дорожно-климатический район: ";
    cin >> dkr;

    int uvl; // схема увлажнения рабочего слоя земляного полотна
    cout << "Выберите схему увлажнения рабочего слоя земляного полотна (1, 2, 3): ";
    cin >> uvl;

    const int TABLE = 3; //количество таблиц в массиве
    const int ROWS = 3; //количество строк в массиве
    const int COLS = 4; //количество колонок в массиве
    double W = 0;

    double Wtab[TABLE][ROWS][COLS] = {
            {
                    {0.63, 0.65, 0.68, 0.73},
                    {0.66, 0.68, 0.71, 0.76},
                    {0.68, 0.70, 0.73, 0.78}
            },
            {
                    {0.60, 0.62, 0.65, 0.70},
                    {0.63, 0.65, 0.68, 0.73},
                    {0.65, 0.67, 0.70, 0.75}
            },
            {
                    {0.65, 0.67, 0.70, 0.75},
                    {0.68, 0.70, 0.73, 0.78},
                    {0.70, 0.72, 0.75, 0.80}
            }
    };

    if (gradation == 6) W = Wtab[dkr - 1][uvl - 1][0];
    else if (gradation == 5) W = Wtab[dkr - 1][uvl - 1][1];
    else if (gradation == 8) W = Wtab[dkr - 1][uvl - 1][2];
    else if (gradation == 7 || gradation == 9) W = Wtab[dkr - 1][uvl - 1][3];

    double wp = W * (1 + 0.1 * t); // формула для расчетного значения влажности грунта Wp, %

    wp = round(wp * 100) / 100; // округление до второго знака после запятой (10 - до первого, 100 - до второго, 1000 - до третьего и тд.)

    return wp;
}

// таблица А1 - расчетные значения характеристик насыпных грунтов
void nas(int gradation) {
    cout << "Выберите тип насыпного грунта: ";
    cin >> gradation;
    double Ey = 0, fn = 0, C = 0, wp = 0, wp_min = 0, wp_max = 0, Ey_min = 0, Ey_max = 0, fn_min = 0, fn_max = 0,
            C_min = 0, C_max = 0;
    int min = 0, max = 0;
    int num;

    if (gradation >=1 && gradation <= 4) {
        num = gradation - 1;
        const int ROWS1_4 = 4; //gradation
        const int COLS1_4 = 3; // Ey, fn, C

        double all_1_4[ROWS1_4][COLS1_4] = {
                {130, 35, 0.004},
                {120, 32, 0.004},
                {100, 31, 0.003},
                {75, 31, 0.003}
        };
        C = all_1_4[num][2];
        fn = all_1_4[num][1];
        Ey = all_1_4[num][0];
    }

    if (gradation >= 5 && gradation <= 9) {
        num = gradation - 5;
        const int ROWS = 5; //количество строк в массиве
        const int COLS = 9; //количество колонок в массиве
        wp = Wp(gradation);

        double Wpt[] = {0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90};

        for (int i = 0; i < COLS; i++) {
            if (wp >= Wpt[i]) {
                min = i;
                max = i + 1;
            }
        }

        double Ey_5_9[ROWS][COLS] = {
                {96, 90, 84, 78, 72, 60, 60, 54, 48},
                {108, 108, 100, 100, 79, 69, 62, 54, 50},
                {108, 108, 100, 100, 79, 69, 62, 54, 50},
                {108, 100, 77, 64, 52, 42, 34, 27, 23},
                {108, 100, 77, 64, 52, 42, 34, 27, 23}
        };
        double fn_5_9[ROWS][COLS] = {
                {38, 38, 37, 37, 36, 35, 34, 33, 32},
                {32, 30, 29, 28, 27, 26, 25, 23, 22},
                {32, 30, 29, 28, 27, 26, 25, 23, 22},
                {27, 25, 23, 21, 19, 18, 16, 13, 11},
                {28, 25, 24, 22, 20, 18, 16, 14, 11}
        };
        double C_5_9[ROWS][COLS] = {
                {0.026, 0.024, 0.022, 0.018, 0.014, 0.012, 0.011, 0.010, 0.009},
                {0.022, 0.016, 0.014, 0.013, 0.011, 0.010, 0.007, 0.005, 0},
                {0.022, 0.020, 0.014, 0.013, 0.011, 0.010, 0.007, 0.005, 0},
                {0.035, 0.026, 0.024, 0.018, 0.014, 0.011, 0.009, 0.006, 0.004},
                {0.040, 0.030, 0.030, 0.019, 0.015, 0.012, 0.009, 0.007, 0.005}
        };

        wp_min = Wpt[min];
        wp_max = Wpt[max];
        C_min = C_5_9[num][min];
        C_max = C_5_9[num][max];
        fn_min = fn_5_9[num][min];
        fn_max = fn_5_9[num][max];
        Ey_min = Ey_5_9[num][min];
        Ey_max = Ey_5_9[num][max];

        C = C_min + (C_max - C_min) / ((wp_max - wp_min) * 100) * (wp - wp_min) * 100;
        fn = fn_min + (fn_max - fn_min) / ((wp_max - wp_min) * 100) * (wp - wp_min) * 100;
        Ey = Ey_min + (Ey_max - Ey_min) / ((wp_max - wp_min) * 100) * (wp - wp_min) * 100;
    }

    string name_gr[] = {"песок крупный", "песок средней крупности", "песок мелкий", "песок однородный", "песок пылеватый",
                        "супесь крупная легкая", "супесь пылеватая", "суглинок легкий НЕ пылеватый", "суглинок легкий пылеватый"};
    int num_n = gradation - 1;
    string name = name_gr[num_n];

    cout << "Тип грунта: " << name << endl;
    cout << "C = " << C << "\t" << "МПа" << endl;
    cout << "fn = " << fn << "\t" << "град" << endl;
    cout << "Ey = " << Ey << "\t" << "МПа" << endl;
    cout << "R0 для насыпных нрунтов не задано" << endl;
}

// wp = round(wp * 100) / 100; // округление до второго знака после запятой (10 - до первого, 100 - до второго, 1000 - до третьего и тд.)
