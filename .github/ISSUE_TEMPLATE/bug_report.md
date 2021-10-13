---
name: Bug report
about: Create a report to help us improve
title: ''
labels: 'state: pending, type: bug'
assignees: ''

---

1. PLEASE CAREFULLY READ: [FAQ](https://github.com/hoffstadt/DearPyGui/discussions/categories/frequently-asked-questions-faq)

2. PLEASE CAREFULLY READ: [Documentation](https://github.com/hoffstadt/DearPyGui/wiki)

3. FOR FIRST-TIME USERS ISSUES,  please use the [Discord server](https://discord.gg/tyE7Gu4).

4. REMOVE ALL 3RD PARTY LIBRARIES.

5. PLEASE MAKE SURE that you have: read the FAQ; explored the examples; searched among Issues; and read the link provided in (1) (2).

6. Be mindful that messages are being sent to the e-mail box of "Watching" users. Try to proof-read your messages before sending them. Edits are not seen by those users.

7. Delete points 1-7 and PLEASE FILL THE TEMPLATE BELOW before submitting your issue or we will immediately close it.

Thank you!

----

## Version of Dear PyGui

Version: XXX
Operating System: XXX (Windows 10, macOs xx, Ubuntu 20.04, etc.)

## My Issue/Question

A clear and concise description of what the issue/question is. Please provide as much context as possible.

## To Reproduce

Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

## Expected behavior

A clear and concise description of what you expected to happen.

## Screenshots/Video

XXX _(you can drag files here)_

## Standalone, minimal, complete and verifiable example

```python
# Here's some code anyone can copy and paste to reproduce your issue
import dearpygui.dearpygui as dpg

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

with dpg.window(label="tutorial"):
    dpg.add_button(label="Press me")

more_code_to_explain_my_issue()

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
```
