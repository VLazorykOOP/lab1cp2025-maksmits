#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// “аблиц€ U(x)
map<double, double> U = {
    {-5.0, 0.2801}, {-4.5, 0.2093}, {-4.0, 0.6190}, {-3.5, 0.8811},
    {-3.0, 1.0422}, {-2.5, 1.1463}, {-2.0, 1.2176}, {-1.5, 1.2560},
    {-1.0, 1.1998}, {-0.5, 1.1209}, {0.0, 1.0039}, {0.5, 0.8196},
    {1.0, 0.5187}, {1.5, 0.0707}, {2.0, 0.4054}, {2.5, 0.7487},
    {3.0, 0.9605}, {3.5, 1.0962}, {4.0, 1.1803}, {4.5, 1.2418}, {5.0, 1.2338}
};

// “аблиц€ T(x)
map<double, double> T = {
    {-10.0, 0.7832}, {-9.0, 1.1063}, {-8.0, 1.2486}, {-7.0, 1.1587},
    {-6.0, 0.9105}, {-5.0, 0.2801}, {-4.0, 0.6190}, {-3.0, 1.0422},
    {-2.0, 1.2176}, {-1.0, 1.1998}, {0.0, 1.0039}, {1.0, 0.5187},
    {2.0, 0.4054}, {3.0, 0.9603}, {4.0, 1.1803}, {5.0, 1.2338},
    {6.0, 1.0761}, {7.0, 0.7068}, {8.0, 0.1450}, {9.0, 0.8533}, {10.0, 1.1347}
};

// “аблиц€ Gtext (≥з text ? x)
map<string, double> Gtext = {
    {"aet", 1.175}, {"bet", 1.278}, {"cet", 1.381}, {"set", 1.484}, {"get", 1.587},
    {"ret", 1.690}, {"het", 1.793}, {"met", 1.896}, {"net", 1.999}, {"qet", 2.102},
    {"tet", 2.205}, {"wet", 2.308}, {"yet", 2.411}, {"oet", 2.617}, {"pet", 2.720},
    {"det", 2.823}, {"fet", 2.926}, {"let", 3.029}, {"zet", 3.132}, {"xet", 3.235}
};

// --- ≤нтерпол€ц≥€
double interpolate(const map<double, double>& table, double x) {
    auto it = table.lower_bound(x);
    if (it != table.end() && it->first == x)
        return it->second;

    auto upper = table.upper_bound(x);
    if (upper == table.begin())
        return upper->second;
    if (upper == table.end())
        return prev(upper)->second;

    auto lower = prev(upper);
    double x0 = lower->first, y0 = lower->second;
    double x1 = upper->first, y1 = upper->second;
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

// --- Qqn
double Qqn(double x, double y, double z) {
    double ux = interpolate(U, x);
    double uy = interpolate(U, z);
    double ty = interpolate(T, y);
    double tz = interpolate(T, z);
    return (x / ux) + (y * ty) - (uy * tz);
}

// --- Qnk
double Qnk(double x, double y) {
    return Qqn(x, y, x + y) - Qqn(y, x, x - y);
}

// --- Rnk
double Rnk(double x, double y) {
    return x * Qnk(x, y) + y * Qnk(y, x);
}

// --- func
double func(double x, double y, double z) {
    double r1 = Rnk(x, y);
    double r2 = Rnk(y, z);
    return r1 + r2 * r1;
}

// --- Max
double Max(double a, double b, double c, double d) {
    return max({ a, b, c, d });
}

// --- CText
double CText(double maxVal, string text, double x) {
    if (text.empty())
        return 0;
    if (Gtext[text] == Gtext["set"])
        return maxVal + x;
    if (Gtext[text] == Gtext["get"])
        return maxVal - x;
    return x;
}

// --- RText
double RText(double x, double y, double z, string text) {
    double maxVal = Max(x, y, x + z, y + z);
    return CText(maxVal, text, x);
}

// --- Variant
double Variant(double r, double k) {
    return 0.8973 * r + 0.1027 * k;
}

// --- main
int main() {
    setlocale(LC_CTYPE, "ukr");
    double x, y, z;
    string text;

    cout << "¬вед≥ть x, y, z: ";
    cin >> x >> y >> z;
    cout << "¬вед≥ть текстовий р€док (наприклад 'wet'): ";
    cin >> text;

    double r = func(x, y, z);
    double k = RText(x, y, z, text);
    double result = Variant(r, k);

    cout << fixed;
    cout.precision(4);
    cout << "\n–езультати обчислень:" << endl;
    cout << "r = " << r << endl;
    cout << "k = " << k << endl;
    cout << "Variant(r, k) = " << result << endl;

    return 0;
}