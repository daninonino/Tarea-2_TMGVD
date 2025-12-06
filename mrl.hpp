#ifndef MRL_HPP
#define MRL_HPP

#include <vector>

class MRLSketch {
public:
    // Construye el sketch con parámetro de error eps y largo esperado del flujo n
    MRLSketch(double epsilon, long long n);

    // Inserta un elemento del flujo
    void insert(long long x);

    // rank(x): número aproximado de elementos < x
    long long rank(long long x) const;

    // select(r): devuelve el valor cuyo rank aproximado es r
    long long select(long long r) const;

    // quantile(phi): phi en [0,1], usa select(floor(phi * N))
    long long quantile(double phi) const;

    // Cantidad de elementos insertados
    long long size() const;

    int get_k() const;
    int get_L() const;

private:
    double eps;                 // parámetro de error ε
    long long n_expected;       // n esperado
    int k;                      // capacidad de cada nivel
    int L;                      // niveles: A_0 .. A_L
    long long total_count;      // número de elementos insertados
    std::vector<std::vector<long long>> levels; // niveles A_0 .. A_L

    void initialize();          // calcula k, L e inicializa niveles
    void compact_level(int j);  // compacta A_j hacia A_{j+1}
};

#endif // MRL_HPP
