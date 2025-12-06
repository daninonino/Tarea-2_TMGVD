#include "mrl.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

MRLSketch::MRLSketch(double epsilon, long long n)
    : eps(epsilon), n_expected(n) {
    initialize();
}

void MRLSketch::initialize() {
    if (eps <= 0.0 || eps >= 1.0) {
        throw std::invalid_argument("eps debe estar en (0,1)");
    }
    if (n_expected <= 0) {
        throw std::invalid_argument("n debe ser positivo");
    }

    // k = eps^{-1} * ceil(log2(eps * n)) + 1 (y se ajusta a par)
    double arg = eps * static_cast<double>(n_expected);

    int log_term;
    if (arg <= 1.0) {
        // para evitar log2 <= 0, usamos 1 como mínimo
        log_term = 1;
    } else {
        log_term = static_cast<int>(std::ceil(std::log2(arg)));
    }

    k = static_cast<int>(std::ceil((1.0 / eps) * log_term)) + 1;

    // asegurar que k sea par
    if (k % 2 != 0) {
        ++k;
    }

    // L = ceil(log2(n / k))
    double ratio = static_cast<double>(n_expected) / static_cast<double>(k);
    if (ratio <= 1.0) {
        L = 0;
    } else {
        L = static_cast<int>(std::ceil(std::log2(ratio)));
    }

    // Inicializar niveles A_0 .. A_L
    levels.assign(L + 1, std::vector<long long>());
    for (int j = 0; j <= L; ++j) {
        levels[j].reserve(k);
    }

    total_count = 0;
}

void MRLSketch::compact_level(int j) {
    auto &Aj  = levels[j];
    auto &Aj1 = levels[j + 1];

    std::sort(Aj.begin(), Aj.end());

    // nos quedamos con índices 0,2,4,... (equiv. impar 1-based)
    int m = static_cast<int>(Aj.size());
    for (int idx = 0; idx < m; idx += 2) {
        Aj1.push_back(Aj[idx]);
    }

    Aj.clear();
}

void MRLSketch::insert(long long x) {
    levels[0].push_back(x);
    ++total_count;

    int j = 0;
    // si algún nivel se llena, se compacta y se propaga hacia arriba
    while (j <= L && static_cast<int>(levels[j].size()) == k) {
        if (j == L) {
            // último nivel: en teoría no debería sobrepasar, pero por si acaso
            break;
        }
        compact_level(j);
        ++j;
    }
}

long long MRLSketch::rank(long long x) const {
    long long ans = 0;
    for (int j = 0; j <= L; ++j) {
        long long weight = (1LL << j); // 2^j
        const auto &Aj   = levels[j];
        for (long long z : Aj) {
            if (z < x) {
                ans += weight;
            }
        }
    }
    return ans;
}

long long MRLSketch::select(long long r) const {
    if (total_count == 0) {
        throw std::runtime_error("select() llamado sobre sketch vacío");
    }

    // Construir B = {(z, 2^j): z in A_j}
    std::vector<std::pair<long long, long long>> B;
    B.reserve(static_cast<std::size_t>(total_count)); // cota superior

    for (int j = 0; j <= L; ++j) {
        long long weight = (1LL << j);
        const auto &Aj   = levels[j];
        for (long long z : Aj) {
            B.emplace_back(z, weight);
        }
    }

    if (B.empty()) {
        throw std::runtime_error("No hay elementos en el sketch");
    }

    std::sort(B.begin(), B.end(),
              [](const auto &a, const auto &b) {
                  return a.first < b.first;
              });

    // Peso total (aprox n)
    long long total_weight = 0;
    for (const auto &p : B) {
        total_weight += p.second;
    }

    if (r <= 0) r = 1;
    if (r > total_weight) r = total_weight;

    long long acc = 0;
    for (const auto &p : B) {
        acc += p.second;
        if (acc >= r) {
            return p.first;
        }
    }

    // Por seguridad devolvemos el mayor
    return B.back().first;
}

long long MRLSketch::quantile(double phi) const {
    if (total_count == 0) {
        throw std::runtime_error("quantile() llamado sobre sketch vacío");
    }

    if (phi < 0.0) phi = 0.0;
    if (phi > 1.0) phi = 1.0;

    long long r = static_cast<long long>(std::floor(phi * total_count));
    if (r <= 0) r = 1;

    return select(r);
}

long long MRLSketch::size() const {
    return total_count;
}

int MRLSketch::get_k() const {
    return k;
}

int MRLSketch::get_L() const {
    return L;
}
