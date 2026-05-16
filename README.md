# ConsoleKit

A lightweight C++17 library for building terminal UIs — spinners, progress bars, tables, panels, and more.

---

## Features

- **Component-based architecture** — Qt-style parent-child ownership
- **Composable layouts** — arrange components vertically or horizontally
- **Animated components** — spinners, activity bars with tick-based updates
- **Rich output** — tables with alignment and row colors, panels with padding and title alignment, text with word wrap
- **Zero dependencies** — pure C++17, no external libraries

---

## Requirements

- C++17 or later
- CMake 3.16+
- ANSI-compatible terminal (Windows Terminal, Linux, macOS)

---

## Installation

### As a subdirectory (recommended)

```cmake
add_subdirectory(path/to/ConsoleKit)
target_link_libraries(MyApp PRIVATE ConsoleKit)
```

### Via CMake install

```bash
mkdir build && cd build
cmake ..
cmake --build .
cmake --install . --prefix ./install
```

Then in your project:

```cmake
find_package(ConsoleKit REQUIRED)
target_link_libraries(MyApp PRIVATE ConsoleKit::ConsoleKit)
```

---

## Project structure

```
ConsoleKit/
├── include/
│   └── ConsoleKit/
│       ├── core/
│       │   ├── Common.h
│       │   ├── Component.h
│       │   ├── Container.h
│       │   ├── Layout.h
│       │   └── ScreenManager.h
│       ├── components/
│       │   ├── ActivityBar.h
│       │   ├── Panel.h
│       │   ├── ProgressBar.h
│       │   ├── Spinner.h
│       │   ├── Table.h
│       │   ├── Text.h
│       │   └── Tree.h
│       └── layouts/
│           ├── HLayout.h
│           └── VLayout.h
├── src/
│   ├── core/
│   ├── components/
│   └── layouts/
└── CMakeLists.txt
```

---

## Quick start

```cpp
#include <ConsoleKit/core/ScreenManager.h>
#include <ConsoleKit/components/Spinner.h>
#include <ConsoleKit/components/ProgressBar.h>
#include <thread>

int main() {
    ck::ScreenManager mgr;

    auto* spinner = new ck::Spinner("Processing...", mgr.getLayout());
    auto* bar = new ck::ProgressBar(0, 100, mgr.getLayout());
    bar->withPercent().withETA().withElapsed();

    for (int i = 0; i <= 100; ++i) {
        bar->update(i);
        mgr.tick();
        mgr.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    spinner->finish("Done!");
    mgr.refresh();
}
```

---

## Architecture

### Ownership model

ConsoleKit follows Qt-style parent-child ownership — when a component is added to a container, the container takes ownership and deletes it on destruction. All components must be created on the heap.

```cpp
// Container owns the component — deletes it automatically
auto* spinner = new ck::Spinner("Loading", mgr.getLayout());

// To transfer or remove a component:
spinner->setParent(nullptr); // detaches without deleting
```

### Render loop

Rendering is separated from state updates:

```cpp
while (true) {
    mgr.tick();    // propagates tick() through the component tree
    mgr.refresh(); // redraws everything
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
}
```

`tick()` advances animations (spinner frames, activity bar position). `refresh()` redraws the full layout to the terminal.

### Layout system

Компоненты образуют дерево. `ScreenManager` владеет корневым `Layout`,
layouts владеют дочерними компонентами, `Panel` владеет одним вложенным компонентом.

- `VLayout` — располагает детей вертикально
- `HLayout` — располагает детей горизонтально
- Layouts можно вкладывать друг в друга
- Листовые компоненты (`Spinner`, `Table`, `Text` и др.) не принимают детей
- `Panel` принимает ровно один дочерний компонент

---

## Components

### ScreenManager

The root of the rendering system. Owns the layout, manages the terminal cursor, and drives refresh.

```cpp
ck::ScreenManager mgr;
mgr.setLayout(new ck::HLayout); // replace default VLayout
mgr.setSpacing(0);              // spacing between components
mgr.tick();
mgr.refresh();
mgr.log("message");             // append a persistent log line below the UI
mgr.setMaxLogs(10);
```

---

### Spinner

Animated spinner with customizable frames and position.

```cpp
auto* s = new ck::Spinner("Loading", mgr.getLayout());
s->setUpdateInterval(80);
s->setPosition(ck::Spinner::Position::Left);
s->setColor(ck::Color::Cyan);
s->finish("Done!", ck::Color::Green);

### ProgressBar

Progress bar with optional statistics.

```cpp
auto* bar = new ck::ProgressBar(0, 100, mgr.getLayout());
bar->withPercent()
   ->withSpeed()
   ->withETA()
   ->withElapsed();

bar->setText("Downloading");
bar->setWidth(40);
bar->setColor(ck::Color::Green);
bar->setFillChar('=');
bar->setHeadChar('>');
bar->setEmptyChar(' ');
bar->onComplete([]{ std::cout << "Done!\n"; });

bar->increment();       // +1
bar->update(50);        // set absolute value
```

---

### ActivityBar

Indeterminate animated bar for tasks without known progress.

```cpp
auto* abar = new ck::ActivityBar("Processing", mgr.getLayout());
abar->setStyle(ck::ActivityBar::Style::Marquee); // Marquee, Pulse, Bounce
abar->setPosition(ck::ActivityBar::Position::Left);
abar->setWidth(30);
abar->setUpdateInterval(50);
abar->setColor(ck::Color::Yellow);
abar->finish("Complete");
```

---

### Table

Data table with borders, alignment, and row colors.

```cpp
auto* t = new ck::Table({"Name", "Age", "City"}, mgr.getLayout());
t->setColor(ck::Color::DarkGrey);
t->setColumnAlign(0, ck::Table::ColumnAlign::Left);
t->setColumnAlign(2, ck::Table::ColumnAlign::Right);
t->setColumnWidth(0, 20);
t->setHeaderVisible(true);

t->addRow({"Anton", "18", "Vilnius"});
t->addRow({"Maria", "24", "Berlin"});

t->setRowColor(0, ck::Color::Green);
t->setAlternatingColors(ck::Color::DarkGrey, ck::Color::Grey);

t->setCell(0, 1, "19");  // update a cell
t->removeRow(1);
t->clear();
```

---

### Panel

Bordered container with title, header text, footer text, and one optional child component.

```cpp
auto* p = new ck::Panel("Title", mgr.getLayout());
p->setTitleAlign(ck::Panel::TitleAlign::Center);
p->setPadding(2, 1);           // horizontal, vertical
p->setColor(ck::Color::Cyan);

p->setHeader("Header text");
p->setFooter("Footer line 1\nFooter line 2");

auto* spinner = new ck::Spinner("Loading", p); // Panel takes ownership

p->clearContent();             // removes child and clears text
```

---

### Text

Multi-line text with word wrap, alignment, and overflow control.

```cpp
auto* txt = new ck::Text("Hello, World!", mgr.getLayout());
txt->setAlign(ck::Text::Align::Center);
txt->setMaxWidth(40);
txt->setOverflow(ck::Text::Overflow::Truncate); // or Wrap
txt->setColor(ck::Color::White);
txt->setText("Updated text");
```

---

### Tree

Hierarchical tree view.

```cpp
auto* tree = new ck::Tree("Root", mgr.getLayout());
auto* child1 = tree->getRoot()->addChild("Child 1");
child1->addChild("Grandchild 1");
child1->addChild("Grandchild 2");
tree->getRoot()->addChild("Child 2");
tree->setColor(ck::Color::Cyan);
```

Output:
```
Root
+-- Child 1
|   +-- Grandchild 1
|   \-- Grandchild 2
\-- Child 2
```

---

### HLayout / VLayout

```cpp
auto* hlay = new ck::HLayout(mgr.getLayout());
hlay->setSpacing(2);

auto* t1 = new ck::Table({"Col"}, hlay);
auto* t2 = new ck::Table({"Col"}, hlay);
```

---

## Colors

```cpp
ck::Color::Black       ck::Color::DarkGrey
ck::Color::Red         ck::Color::LightRed
ck::Color::Green       ck::Color::LightGreen
ck::Color::Yellow      ck::Color::LightYellow
ck::Color::Blue        ck::Color::LightBlue
ck::Color::Magenta     ck::Color::LightMagenta
ck::Color::Cyan        ck::Color::LightCyan
ck::Color::Grey        ck::Color::White
```

---

## License

MIT
