| [Basics](#basics) - [Example](#example) - [0.6.x](https://github.com/hoffstadt/DearPyGui_06/wiki/Item-Configuration) |
|----|
||

## Basics
In _Dear PyGui_, when GUI items are added, various configuration options and flags can be set. There are several options common to all items (i.e. **show**) but most GUI items have specific options. In order to modify an item's configuration after being created, you can use the `configure_item` command in conjunction with the keyword from the item's `add_*` command. You can also retrieve an item's configuration in the form of a dictionary by using the `get_item_configuration` command.

## Example
Simple usage can be found below:

```python
add_button(enabled=True, label="Press me", id="item")

# at a later time, change the item's configuration
configure_item("item", enabled=False, label="New Label")
```