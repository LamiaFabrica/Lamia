# Lamia 0.5.0 — Compiler and Transpiler Options

**© 2025 D Hargreaves AKA Roylepython | All Rights Reserved**

The compiler and transpiler expose options so you can control output and parsing.

---

## CompilerOptions (C++ API)

Used with `lamia::compiler::Compiler::set_options()`.

| Member | Type | Default | Description |
|--------|------|---------|-------------|
| `target` | std::string | `"html"` | Output target: `html`, `js`, or `gcode` (html fully supported in 0.5.0) |
| `pretty_print` | bool | true | Emit indented, readable output; if false, output is compact |
| `emit_comments` | bool | false | Emit source-location or generator comments in output |
| `strict` | bool | false | Strict parsing: reject unknown statements with an error |

### Example

```cpp
import lamia.core;
import lamia.compiler;

lamia::compiler::Compiler c;
lamia::compiler::CompilerOptions opts;
opts.target = "html";
opts.pretty_print = true;
opts.emit_comments = true;
opts.strict = false;
c.set_options(opts);

auto r = c.compile("manifest app { create RADIANT_HEADING { content: \"Hello\" } }");
```

---

## TranspilerOptions (C++ API)

Used with `lamia::compiler::Transpiler::set_options()` when using the transpiler directly.

| Member | Type | Default | Description |
|--------|------|---------|-------------|
| `target` | std::string | `"html"` | Output target |
| `pretty_print` | bool | true | Indented output |
| `emit_comments` | bool | false | Generator/source comments in output |

---

## Backward compatibility

- `Compiler::set_target(std::string)` still works; it sets `options_.target`.
- Default behaviour (html, pretty print, no comments, non-strict) matches 0.4.x.

---

## Future targets (0.5.x+)

- **js** — Emit JavaScript instead of HTML (planned).
- **gcode** — Emit G-code for manufacturing (planned).

Current 0.5.0 implementation uses the same HTML emission; options control formatting and comments only.
