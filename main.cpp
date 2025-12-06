#include "mrl.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc < 4) {
        std::cerr << "Uso: " << argv[0]
                  << " <n> <eps> <archivo_flujo>\n";
        std::cerr << "  n:    largo esperado del flujo\n";
        std::cerr << "  eps:  error (por ejemplo 0.1 o 0.05)\n";
        std::cerr << "  archivo_flujo: archivo con los datos (uno por línea)\n";
        return 1;
    }

    long long n      = std::stoll(argv[1]);
    double eps       = std::stod(argv[2]);
    std::string file = argv[3];

    try {
        MRLSketch sketch(eps, n);

        std::ifstream in(file);
        if (!in) {
            std::cerr << "No se pudo abrir el archivo: " << file << "\n";
            return 1;
        }

        long long x;
        long long count = 0;
        while (in >> x) {
            sketch.insert(x);
            ++count;
        }
        in.close();

        if (count != n) {
            std::cerr << "Advertencia: se leyeron " << count
                      << " elementos, pero n = " << n << "\n";
        }

        std::cerr << "Sketch construido.\n";
        std::cerr << "Elementos insertados: " << sketch.size()
                  << ", k = " << sketch.get_k()
                  << ", L = " << sketch.get_L() << "\n\n";

        std::cerr << "Modo interactivo.\n";
        std::cerr << "  rank x        -> estima rank(x)\n";
        std::cerr << "  quantile phi  -> estima cuantil(phi), phi en [0,1]\n";
        std::cerr << "  exit          -> salir\n\n";

        std::string cmd;
        while (true) {
            std::cerr << "> ";
            if (!(std::cin >> cmd)) break;
            if (cmd == "exit" || cmd == "salir") break;

            if (cmd == "rank") {
                long long v;
                std::cin >> v;
                long long r = sketch.rank(v);
                std::cout << "rank(" << v << ") ≈ " << r << "\n";
            } else if (cmd == "quantile") {
                double phi;
                std::cin >> phi;
                long long q = sketch.quantile(phi);
                std::cout << "quantile(" << phi << ") ≈ " << q << "\n";
            } else {
                std::cerr << "Comando no reconocido. Usa rank, quantile o exit.\n";
            }
        }

    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
