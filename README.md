# 🧮 Shunting Yard Calculator (GTK Demo)

A basic calculator built using C and GTK that uses the **Shunting Yard Algorithm** to evaluate infix expressions (e.g. `3 + 4 * 2 / (1 - 5)`). This project demonstrates tokenization, infix-to-postfix conversion, and stack-based evaluation in a GUI.

---

## 📸 Demo

![Calculator Demo](/assets/demo.gif)

---

## 🛠 Features

- GUI built with GTK (C)
- Custom implementation of:
  - Token stack
  - Operation stack
  - Evaluation stack
- Handles `+`, `-`, `*`, `/` with proper **operator precedence**
- Manual input building per button click
- Evaluates using **postfix (RPN)** logic

---

## ⚠️ Disclaimer

> **This project is for demo and educational purposes only.**
>
> It may contain bugs or incomplete logic (e.g., limited error handling, no parentheses support). Do not use it in production or rely on its correctness for real-world calculations.

---

## 🧰 Requirements

- GTK 3 or 4 development libraries
- A C compiler (e.g. `gcc`)

---

## 🧪 Building & Running

```bash
make
./bin/calculator
```
