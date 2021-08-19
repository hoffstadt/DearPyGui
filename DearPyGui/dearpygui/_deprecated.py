
def enable_docking(dock_space: bool = False) -> None:
    """'enable_docking(...)' is deprecated. Use 'configure_app(docking=True, docking_space=dock_space)'."""
    warnings.warn("'enable_docking(...)' is deprecated. Use 'configure_app(docking=True, docking_space=dock_space)'.", DeprecationWarning, 2)
    internal_dpg.configure_app(docking=True, docking_space=dock_space)


def get_dearpygui_version() -> str:
    """'get_dearpygui_version()' is deprecated. Use 'get_app_configuration()['version']'."""
    warnings.warn("'get_dearpygui_version()' is deprecated. Use 'get_app_configuration()['version']'.", DeprecationWarning, 2)
    return internal_dpg.get_app_configuration()["version"]


def set_init_file(file: str = "dpg.ini") -> None:
    """'init_file(...)' is deprecated. Use 'configure_app(init_file=file)'."""
    warnings.warn("'init_file(...)' is deprecated. Use 'configure_app(init_file=file)'.", DeprecationWarning, 2)
    internal_dpg.configure_app(init_file=file)


def load_init_file(file: str) -> None:
    """'load_init_file' is deprecated. Use 'configure_app(init_file=file, load_init_file=True)'."""
    warnings.warn("'load_init_file' is deprecated. Use 'configure_app(init_file=file, load_init_file=True)'.", DeprecationWarning, 2)
    internal_dpg.configure_app(init_file=file, load_init_file=True)

def is_viewport_created() -> bool:
    """
    'is_viewport_created' is deprecated. Use: `is_viewport_ok(...)`
    Checks if a viewport has been created and shown.
    Returns:
        Bool
    """
    warnings.warn("'is_viewport_created' is deprecated. Use: `is_viewport_ok(...)`", DeprecationWarning, 2)
    return internal_dpg.is_viewport_ok()


def setup_viewport():
    """
    'setup_viewport(...)' is deprecated. Use:
    create_viewport()
    setup_dearpygui()
    show_viewport()

    Prepares viewport.

    Returns:
        None
    """
    warnings.warn("'setup_viewport' is deprecated. Use: \ncreate_viewport()\nsetup_dearpygui()\nshow_viewport()", DeprecationWarning, 2)
    internal_dpg.create_viewport()
    internal_dpg.setup_dearpygui()
    internal_dpg.show_viewport()


def set_item_theme(item : Union[int, str], theme : Union[int, str]) -> None:
	"""
	Undocumented
	Args:
		item (Union[int, str]): 
		theme (Union[int, str]): 
	Returns:
		None
	"""
	warnings.warn("'set_item_theme' is deprecated. Use: `bind_item_theme(...)`", DeprecationWarning, 2)
	return internal_dpg.bind_item_theme(item, theme)


def set_item_type_disabled_theme(item : int, theme : Union[int, str]) -> None:
	"""
	Undocumented
	Args:
		item (int): 
		theme (Union[int, str]): 
	Returns:
		None
	"""
	warnings.warn("'set_item_type_disabled_theme' is deprecated. Use: `bind_item_type_disabled_theme(...)`", DeprecationWarning, 2)
	return internal_dpg.bind_item_type_disabled_theme(item, theme)


def set_item_type_theme(item : int, theme : Union[int, str]) -> None:
	"""
	Undocumented
	Args:
		item (int): 
		theme (Union[int, str]): 
	Returns:
		None
	"""
	warnings.warn("'set_item_type_theme' is deprecated. Use: `bind_item_type_theme(...)`", DeprecationWarning, 2)
	return internal_dpg.bind_item_type_theme(item, theme)


def set_item_font(item : Union[int, str], font : Union[int, str]) -> None:
	"""
	Undocumented
	Args:
		item (Union[int, str]): 
		font (Union[int, str]): 
	Returns:
		None
	"""
	warnings.warn("'set_item_font' is deprecated. Use: `bind_item_font(...)`", DeprecationWarning, 2)
	return internal_dpg.bind_item_font(item, font)


def add_activated_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is activated.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_activated_handler' is deprecated. Use: `add_item_activated_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_activated_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_active_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is active.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_active_handler' is deprecated. Use: `add_item_active_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_active_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_clicked_handler(parent : Union[int, str], button : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is clicked.
	Args:
		parent (Union[int, str]): 
		*button (int): Submits callback for all mouse buttons
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_clicked_handler' is deprecated. Use: `add_item_clicked_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_clicked_handler(parent, button, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_deactivated_after_edit_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is deactivated after edit.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_deactivated_after_edit_handler' is deprecated. Use: `add_item_deactivated_after_edit_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_deactivated_after_edit_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_deactivated_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is deactivated.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_deactivated_handler' is deprecated. Use: `add_item_deactivated_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_deactivated_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_edited_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is edited.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_edited_handler' is deprecated. Use: `add_item_edited_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_edited_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_focus_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is focused.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_focus_handler' is deprecated. Use: `add_item_focus_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_focus_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_hover_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is hovered.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_hover_handler' is deprecated. Use: `add_item_hover_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_hover_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_resize_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is resized.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""

	warnings.warn("'add_resize_handler' is deprecated. Use: `add_item_resize_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_resize_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_toggled_open_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is toggled open.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_toggled_open_handler' is deprecated. Use: `add_item_toggled_open_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_toggled_open_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def add_visible_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	"""
	Adds a handler which runs a given callback when the specified item is visible.
	Args:
		parent (Union[int, str]): 
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		**callback (Callable): Registers a callback.
		**show (bool): Attempt to render widget.
	Returns:
		Union[int, str]
	"""
	warnings.warn("'add_visible_handler' is deprecated. Use: `add_item_visible_handler(...)`", DeprecationWarning, 2)
	return internal_dpg.add_item_visible_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, id=id, callback=callback, show=show)


def set_colormap(item : Union[int, str], source : Union[int, str]) -> None:
	"""
	Sets the color map for widgets that accept it.
	Args:
		item (Union[int, str]): 
		source (Union[int, str]): 
	Returns:
		None
	"""
	warnings.warn("'set_colormap' is deprecated. Use: `bind_colormap(...)`", DeprecationWarning, 2)
	return internal_dpg.bind_colormap(item, source)


def reset_default_theme(item : Union[int, str], source : Union[int, str]) -> None:
	"""
	Resets default theme
	Args:
		item (Union[int, str]): 
		source (Union[int, str]): 
	Returns:
		None
	"""
	warnings.warn("'reset_default_theme' is deprecated. Use: `bind_theme(0)`", DeprecationWarning, 2)
	return internal_dpg.bind_theme(item, source)