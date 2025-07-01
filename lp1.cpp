#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>
#include <stdexcept>

using namespace std;

// --- Класи виключень
class MyRangeError : public exception {
    double x;
public:
    MyRangeError(double x_) : x(x_) {}
    void Message() const {
        cout << "[Помилка діапазону]: Значення " << x << " вийшло за допустимі межі." << endl;
    }
};

class TableLookupError : public exception {
    double x;
public:
    TableLookupError(double x_) : x(x_) {}
    void Message() const {
        cout << "[Помилка інтерполяції]: Не вдалося знайти інтервал для x = " << x << endl;
    }
};

class TextKeyError : public exception {
    string key;
public:
    TextKeyError(const string& k) : key(k) {}
    void Message() const {
        cout << "[Помилка тексту]: Ключ \"" << key << "\" відсутній у таблиці Gtext.\n";
    }
};

// --- Таблиці
map<double, double> U = {
    {-5.0, 0.2801}, {-4.5, 0.2093}, {-4.0, 0.6190}, {-3.5, 0.8811}, {-3.0, 1.0422},
    {-2.5, 1.1463}, {-2.0, 1.2176}, {-1.5, 1.2560}, {-1.0, 1.1998}, {-0.5, 1.1209},
    {0.0, 1.0039}, {0.5, 0.8196}, {1.0, 0.5187}, {1.5, 0.0707}, {2.0, 0.4054},
    {2.5, 0.7487}, {3.0, 0.9605}, {3.5, 1.0962}, {4.0, 1.1803}, {4.5, 1.2418}, {5.0, 1.2338}
};

map<double, double> T = {
    {-10.0, 0.7832}, {-9.0, 1.1063}, {-8.0, 1.2486}, {-7.0, 1.1587}, {-6.0, 0.9105},
    {-5.0, 0.2801}, {-4.0, 0.6190}, {-3.0, 1.0422}, {-2.0, 1.2176}, {-1.0, 1.1998},
    {0.0, 1.0039}, {1.0, 0.5187}, {2.0, 0.4054}, {3.0, 0.9603}, {4.0, 1.1803},
    {5.0, 1.2338}, {6.0, 1.0761}, {7.0, 0.7068}, {8.0, 0.1450}, {9.0, 0.8533}, {10.0, 1.1347}
};

map<string, double> Gtext = {
    {"aet", 1.175}, {"bet", 1.278}, {"cet", 1.381}, {"set", 1.484}, {"get", 1.587},
    {"ret", 1.690}, {"het", 1.793}, {"met", 1.896}, {"net", 1.999}, {"qet", 2.102},
    {"tet", 2.205}, {"wet", 2.308}, {"yet", 2.411}, {"oet", 2.617}, {"pet", 2.720},
    {"det", 2.823}, {"fet", 2.926}, {"let", 3.029}, {"zet", 3.132}, {"xet", 3.235}
};

// --- Інтерполяція
double interpolate(const map<double, double>& table, double x) {
    if (x < table.begin()->first || x > table.rbegin()->first)
        throw MyRangeError(x);

    auto it = table.lower_bound(x);
    if (it != table.end() && it->first == x)
        return it->second;

    auto upper = table.upper_bound(x);
    if (upper == table.begin() || upper == table.end())
        throw TableLookupError(x);

    auto lower = prev(upper);
    double x0 = lower->first, y0 = lower->second;
    double x1 = upper->first, y1 = upper->second;
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

double Qqn(double x, double y, double z) {
    return (x / interpolate(U, x)) + (y * interpolate(T, y)) - (interpolate(U, z) * interpolate(T, z));
}

double Qnk(double x, double y) {
    return Qqn(x, y, x + y) - Qqn(y, x, x - y);
}

double Rnk(double x, double y) {
    return x * Qnk(x, y) + y * Qnk(y, x);
}

double func(double x, double y, double z) {
    return Rnk(x, y) + Rnk(y, z) * Rnk(x, y);
}

double Max(double a, double b, double c, double d) {
    return max({ a, b, c, d });
}

double CText(double maxVal, string text, double x) {
    auto it = Gtext.find(text);
    if (it == Gtext.end())
        throw TextKeyError(text);

    if (text == "set")
        return maxVal + x;
    else if (text == "get")
        return maxVal - x;
    return x;
}

double RText(double x, double y, double z, string text) {
    double maxVal = Max(x, y, x + z, y + z);
    return CText(maxVal, text, x);
}

double Variant(double r, double k) {
    return 0.8973 * r + 0.1027 * k;
}

// --- Основна програма
int main() {
    setlocale(LC_ALL, "ukr");
    double x, y, z;

    cout << "Введіть x, y, z: ";
    cin >> x >> y >> z;
    string text;
    cout << "Введіть текстовий рядок (наприклад 'wet'): ";
    cin >> text;
    try {
        double r = func(x, y, z);
        double k = RText(x, y, z, text);
        double result = Variant(r, k);

        cout << fixed;
        cout.precision(4);
        cout << "\nРезультати обчислень:" << endl;
        cout << "r = " << r << endl;
        cout << "k = " << k << endl;
        cout << "Variant(r, k) = " << result << endl;
    }
    catch (MyRangeError& e) {
        e.Message();
    }
    catch (TextKeyError& e) {
        e.Message();
    }
    catch (TableLookupError& e) {
        e.Message();
    }
    catch (exception& e) {
        cout << "Виникла невідома помилка: " << e.what() << endl;
    }

    return 0;
}