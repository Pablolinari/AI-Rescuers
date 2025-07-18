# Práctica 2 de *Inteligencia Artificial*, curso 2024/2025
Artificial Intelligence Course - First Project Description

This project is the first project of the Artificial Intelligence course. It consists of a game where a main rescuer and an auxiliary rescuer are involved, and they must achieve different objectives depending on the level they are on.

There are two types of agents , main and auxiliary and various types of cells, such as bush, water, paths, roads, shoes, safety, and forest. Each cell type influences the actions that agents can perform. Additionally, agents have different capabilities; for example, the main agent can run, and the auxiliary agent can move through bushes if it is wearing shoes.

Each cell is assigned a height. If the height difference between cells exceeds what an agent can handle, the agent will either bump into the cell or fall and die, depending on the situation.





Level 0: Both agents are reactive agents. They must navigate the given maps and reach their respective checkpoints before their energy is depleted or time runs out.The agents don't know the map.



Level 1: Both agents are reactive and must explore the entire map, but they can only move through designated paths and roads.The agents don't know the map.



Level 2: The main agent must use the Dijkstra algorithm to find the most efficient path to reach the goal cell.The agents know the map.



Level 3: The auxiliary agent must use the A* algorithm to find the most efficient path to reach the goal cell.The agents  know the map.



Level 4: Both agents use reactive and deliberative behaviors. The main agent must reach the cell where the accident is located. If the problem is severe, the main agent must call the auxiliary agent and wait for them to arrive. The goal of this level is to earn as many points as possible. Points are awarded when a rescue is completed. Once a rescue is completed, the agents can move on to another rescue. The main agent can decide whether or not to call the auxiliary agent. The simulation ends when the agents' battery reaches zero or the simulation time expires. The agents don't know the map.
## Prerrequisitos

### Crear una cuenta en [GitHub](https://github.com/). 
Para ello, puedes usar tu correo personal, el de *@correo.ugr.es* o el de *@go.ugr.es*.


### 1. Añadir tu clave SSH a GitHub
Hay varias maneras de conectarte desde tu ordenador a GitHub. Si utilizas un navegador, usarás tu usuario y contraseña. Desde el terminal, lo más cómodo es utilizar una clave SSH. Puedes crear una nueva si no tienes, o reutilizar una ya existente. Tienes toda la información para realizar la configuración en: 
[Conectar a GitHub con SSH](https://docs.github.com/es/authentication/connecting-to-github-with-ssh)


### 2. Crear tu copia personal del repositorio de la asignatura
Cada estudiante debe tener su propia copia del repositorio para poder trabajar sobre ella. En adelante, a tu copia la llamaremos *origin*, y al repositorio original de la asignatura lo llamaremos *upstream* (NOTA: Estas son convenciones que la mayoría de los desarrolladores usan, pero los puedes llamar como quieras). 

> La forma usual de crear tu copia del repositorio es realizando un *fork*. Sin embargo, dado que realizar un *fork* de un repositorio con visibilidad pública obliga al que la copia sea también pública, nosotros usaremos un procedimiento diferente que nos permite que nuestra copia del repositorio sea privada.

Para realizar la copia, una vez que tengas creada tu cuenta en GitHub, haz click en <https://github.com/new/import> y rellena tal y como se ve en la imagen de abajo. El repositorio que quieres importar es `https://github.com/ugr-ccia-IA/practica2`. ¡Asegúrate de que tu repositorio es privado!

![Importar repositorio practica2](doc/img/import_new_repo.png)


### 3. Clonar tu repositorio en tu máquina
Una vez hecho el paso anterior, tendrás tu repositorio personal de la práctica1 en GitHub; puedes descargarlo a tu ordenador usando:
`git clone git@github.com:TU_USUARIO_GITHUB/practica2.git` (si no has configurado tu clave SSH, esto no funcionará).


### 4. Modificar el código y guardar los cambios
Es el momento de empezar a modificar ficheros. Abre el fichero README.md (este fichero), ve al final y añade una línea que diga "Esto lo puse yo."
Una vez lo hayas modificado, guarda el fichero, y ejecuta los siguientes comandos en el terminal estando dentro de la carpeta `practica2`:

```
git add . 
git commit -m "Modificando README.md"
git push origin main 
```

Los tres comandos anteriores le indican a git que 1) queremos guardar una nueva versión con todos los ficheros modificados de la carpeta, 2) que haga esa versión y le ponga el comentario "Cambiando el enlace del botón", y 3) que envíe esta nueva versión a la copia de nuestro repositorio alojada en GitHub.

Este proceso es el que debes repetir cada vez que vayas avanzando en la implementación de la práctica: add, commit, push.



### 5. Enlazar tu repositorio personal con el de la asignatura
Aunque tu repositorio y el de la asignatura (recuerda que los conocemos por *origin* y *upstream* respectivamente) sean independientes, nos va a interesar que estén enlazados. De esta forma, podrás aplicar fácilmente sobre tu repositorio (*origin*) cualquier actualización que los profesores realicemos en *upstream*. Para enlazarlos, ejecuta lo siguiente dentro de la carpeta de tu repositorio:

`git remote add upstream git@github.com:ugr-ccia-IA/practica2.git`


### Actualizar tu repositorio con cambios realizados en el de la asignatura
Una vez tengas los repositiorios enlazados, lo único que debes hacer para aplicar posibles cambios en el repositorio de la asignatura en tu repositorio (cambios de *upstream* en *origin*) es: `git pull upstream main`

Hacer esto no sobreescribirá tus avances en la implementación de la práctica, puesto que tú no deberías haber modificado ninguna parte del código diferente a la que se indica en el guión.

Si quieres que esos cambios también se guarden en github, a continuación ejecuta: `git push origin main`


> Si quieres saber más sobre Git y GitHub, en Internet existen multitud de recursos, incluidos videos y tutoriales. Para realizar esta práctica sólo necesitas lo básico (hacer commits), pero hay muchas cosas más que se pueden hacer con estas herramientas (uso de ramas, gestión de conflictos, etc.) 
El propio GitHub pone a tu disposición un [breve curso](https://classroom.github.com/a/W33pQ3pa) (en inglés) para aprender lo básico.


## Realización de la práctica
El guión (disponible en [PRADO](https://pradogrado2425.ugr.es/)) contiene toda la información sobre en qué consiste la práctica2. Leelo con atención.

Junto a ellos, también tienes a tu disposición una pequeña presentación de resumen, y un tutorial. Debes revisarlos pues continen los primeros pasos a realizar.


### Instalación local (linux)

Una vez que tengas tu repositorio (el fork que has realizado) en tu ordenador, puedes compilar el código usando `./install.sh` (esto instalará todas las dependencias, y ejecutará `cmake` y `make`. ).
A continuación, puedes lanzar el software con interfaz gráfica con `./practica2`, o sin ella con `./practica2SG`.

Cuando realices cualquier modificación en el código, debes recompilar, así que usa `make clean` y `make`.


### Instalación en máquina virtual
Si no tienes linux de forma nativa en tu ordenador, puedes crear una máquina virtual en tu ordenador (por ejemplo, usando VirtualBox), e instalar linux en ella. A partir de ahí, puedes trabajar dentro de ella como si tuvieras linux instalado de forma nativa.


## Más información
Hemos creado un [fichero con preguntas frecuentes](./FAQ.md) que han ido apareciendo en las distintas sesiones de prácticas.

