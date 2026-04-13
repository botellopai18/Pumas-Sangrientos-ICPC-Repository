# 🐾 Pumas Sangrientos - ICPC Team Reference

> **"Forjando algoritmos, destrozando Time Limits y dejando huella en cada Accepted."**

¡Bienvenidos a la guarida oficial de los **Pumas Sangrientos**! 🩸🐆 
Este repositorio es nuestra biblioteca de armas letales para la Programación Competitiva. Aquí almacenamos, pulimos y automatizamos los algoritmos que nos llevarán a la victoria en la ICPC. 

No escribimos código al azar; construimos una máquina bien engrasada. Nuestro Team Reference se genera **automáticamente** a partir de nuestros códigos fuente, asegurando que siempre tengamos un PDF limpio, estructurado y listo para imprimir antes de cualquier competencia.

---

## ⚙️ ¿Cómo funciona nuestra magia?

Olvídate de pelear con LaTeX manualmente. Tenemos un **Code Pruner & Generador Automático**:
1. Escribimos nuestro código en C++ (o Python/Java) dentro de la carpeta `code/`.
2. Ejecutamos nuestro script `generator.py`.
3. El script purga la basura (librerías repetidas, fast_io), extrae los algoritmos puros y construye un `notebook.pdf` a dos columnas con índice generado al vuelo.

---

## 🛠️ Guía de Aportación: Cero Conflictos

Para que el repositorio no se rompa y evitemos el temido `Merge Conflict` (especialmente en los archivos `.pdf` y `.tex`), **todo el equipo debe seguir exactamente este flujo de trabajo:**

### Paso 1: Sincroniza antes de respirar
Antes de escribir una sola línea de código, asegúrate de tener la última versión del repositorio.
```bash
git pull origin main
```

### Paso 2: Agrega tu código con Metadatos
Crea o edita tu archivo dentro de la carpeta correspondiente en `code/` (ej. `code/math/criba.cpp`). 
Para que el generador reconozca tu código, **debes** incluir este encabezado exacto en la parte superior:

```cpp
/**
 * Title: Nombre de tu Algoritmo (Opcional, si no, usa el nombre del archivo)
 * Description: ¿Qué hace? ¿Cuál es su complejidad? (Puedes usar math de LaTeX aquí entre $$)
 * Author: Tu Nombre/Apodo
 */
```

### Paso 3: Genera el PDF Localmente
Una vez que tu código está listo y probado, corre el generador. Esto actualizará el `notebook.tex` y creará el nuevo `notebook.pdf` con tus cambios.
```bash
python generator.py
```
*(Nota: El script limpiará automáticamente los archivos residuales de LaTeX).*

### Paso 4: Sube tus cambios limpios
Añade tus archivos fuente y el PDF actualizado, haz el commit y empuja a la rama principal.
```bash
git add code/ generator.py template.tex
git add -f notebook.tex notebook.pdf
git commit -m "Agregado algoritmo X a la sección Y"
git push origin main
```

---

## 🚫 Las 3 Reglas de Oro para evitar el Caos

1. **NUNCA edites `notebook.tex` a mano.** Cualquier cambio manual será sobreescrito la próxima vez que alguien corra el generador de Python. Si quieres cambiar el diseño, edita `template.tex`.
2. **Si el `git push` falla**, significa que alguien más subió código mientras tú trabajabas. No entres en pánico:
   * Haz `git pull origin main`.
   * Vuelve a correr `python generator.py` (para que el PDF combine tu código y el de tu compañero).
   * Vuelve a hacer `git commit` y `git push`.
3. **Mantén tu código limpio.** Asegúrate de que compile antes de subirlo. Un reference con código que no compila es un reference inútil.

---
**¡A codear, Pumas! Nos vemos en el scoreboard. 🏆**