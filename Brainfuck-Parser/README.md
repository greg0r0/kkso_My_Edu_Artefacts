# Brainfuck-Parser
Program for creating AST (in XML format) from Brainfuck source code
# Example
Before:

```--[++--]++```

After:
```
<?xml version="1.0"?>
<program>
	<t_DECREASE />
	<t_DECREASE />
	<t_LOOP>
		<t_INCREASE />
		<t_INCREASE />
		<t_DECREASE />
		<t_DECREASE />
	</t_LOOP>
	<t_INCREASE />
	<t_INCREASE />
</program>
```