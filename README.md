
```markdown
# 🤖 Práctica 2 – Agente Basado en Objetivos

**Asignatura:** Inteligencia Artificial – Curso 2023/2024  
**Autor:** [Tu nombre]

---

## 📌 Introducción
En esta práctica se implementa un **agente basado en objetivos** para el entorno *Los mundos de BelKan*. El agente debe ser capaz de:

-   🌍 Explorar el mapa.
-   📍 Localizar y alcanzar objetivos concretos.
-   🐺 Evitar obstáculos y enemigos.
-   🔋 Gestionar recursos como la batería y objetos especiales.

A diferencia de la Práctica 1 (agente puramente reactivo), aquí se incorpora **planificación de acciones** para llegar a objetivos y adaptarse dinámicamente a cambios en el entorno.

---

## 🗺️ Entorno y Sensores
El entorno es un mapa con diferentes tipos de terrenos y elementos:

-   **Terrenos:** Bosques (`B`), Agua (`A`), Precipicios (`P`), Arena (`T`), Suelo normal (`S`), Muros (`M`).
-   **Objetos:** Bikini (`K`), Zapatillas (`D`), Recarga de batería (`X`), Casillas de posicionamiento (`G`).
-   **Agentes móviles:** Aldeanos (`a`), Lobos (`l`).

#### Sensores del agente:
-   Terreno y agentes visibles en un radio de visión.
-   Colisión, `reset`, posición y orientación.
-   Nivel de batería, nivel actual y tiempo restante.

---

## 🎯 Metodología y Niveles

### Nivel 0 – Con posición y orientación
-   Planificación directa desde la posición inicial al objetivo usando **Búsqueda en Anchura (BFS)** sobre el mapa visible.
-   Actualización constante del mapa interno del agente.

### Nivel 1 – Sin sensor de posición
-   Estimación de la posición inicial mediante desplazamientos y referencias del terreno.
-   Uso de **BFS** una vez determinada la posición estimada.
-   ⚠️ **Problema detectado:** Se encontró una comprobación innecesaria en la acción `actCLB_TURN_SR` para casillas transitables, lo que afectó negativamente a los giros en algunos casos.

### Nivel 2 – Con enemigos y obstáculos móviles
-   Incorporación de **evasión dinámica** frente a aldeanos y lobos detectados.
-   La planificación rehace la ruta si el camino original queda bloqueado.

### Nivel 3 – Sin orientación inicial
-   Rutina de exploración para determinar la orientación basándose en patrones de terreno y referencias visuales.
-   Ajuste y planificación de la ruta una vez orientado.

### Nivel 4 – Sensores limitados
-   Mayor incertidumbre: planificación con información parcial y sensores ruidosos.
-   Estrategia híbrida que combina exploración y planificación según la confianza en los datos.

---

## ⚙️ Implementación

-   **Lenguaje:** C++
-   **Ficheros clave:**
    -   `jugador.cpp`: Implementación de la clase `ComportamientoJugador` con la lógica de decisión y planificación.
    -   `jugador.hpp`: Definición de la clase y métodos auxiliares.

#### Planificación:
-   Uso de **Búsqueda en Anchura (BFS)** para obtener la ruta óptima (en número de pasos) al objetivo.
-   Mantenimiento de una representación interna del mapa para registrar terrenos explorados y obstáculos.

#### Decisiones clave:
-   Evitar muros y precipicios de forma reactiva.
-   Priorizar la obtención de objetos clave (bikini y zapatillas) antes de cruzar terrenos costosos.
-   Gestionar la batería, iniciando una búsqueda de recargas (`X`) cuando el nivel es bajo.

---

## 📈 Resultados
-   Se obtuvo un alto porcentaje de éxito en los niveles 0, 2, 3 y 4.
-   En el **nivel 1**, el comportamiento se vio afectado por la condición extra en los giros (`actCLB_TURN_SR`), un error que fue detectado y comentado en la corrección de la práctica.
-   La planificación con **BFS** resultó muy eficiente en mapas de tamaño mediano, con tiempos de respuesta adecuados para el simulador.

---

## 📝 Conclusiones
-   Un **agente basado en objetivos** mejora significativamente la capacidad de cumplir metas concretas en entornos complejos.
-   La gestión de un **mapa interno** y el uso de **planificación** permiten comportamientos mucho más inteligentes y adaptativos que el enfoque puramente reactivo.
-   La correcta gestión de las precondiciones en las acciones es crucial para evitar bloqueos y comportamientos no deseados.
-   **Posible mejora:** Incorporar algoritmos de planificación más avanzados como **A***, utilizando heurísticas de coste (batería, tiempo) para optimizar rutas en entornos más grandes o con recursos más limitados.
```
