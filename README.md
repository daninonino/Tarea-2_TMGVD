# Tarea-2_TMGVD
# Sketch de Cuantiles MRL

Implementación en C++ del sketch de cuantiles de Manku–Rajagopalan–Lindsay (MRL)
para el modelo de *data stream*. El objetivo es estimar operaciones de `rank(x)` y
`quantile(ϕ)` sobre flujos de datos grandes usando espacio sublineal.

## Estructura del proyecto

```text

├─ main.cpp        # Programa principal (lee el flujo y responde consultas)
├─ mrl.cpp         # Implementación de la clase MRLSketch
├─ mrl.hpp         # Header de la clase MRLSketch
├─ chicago2015.txt
├─ chicago2016.txt
├─ Log-normal.txt
└─ Uniform.txt
```
## Compilación

```
g++ -std=c++17 -O2 main.cpp mrl.cpp -o mrl_sketch
```
## Ejecución
```
./mrl_sketch <n> <eps> <archivo_flujo>
```
Donde: 
- **n** → número de líneas del archivo (tamaño del flujo)
- **eps** → parámetro de error (ej: `0.1`, `0.05`)
- **archivo_flujo** → ruta al archivo `.txt`
  
## Modo Interactivo
Una vez cargado el archivo, el programa permite hacer consultas en la consola. Con los comandos ```rank x``` y ```quantile(ϕ)``` se puede pedir en consola los datos para una consulta. Para cerrar el modo interactivo se debe escribir el comando ```quit```.
