
def deprecated(reason):

	string_types = (type(b''), type(u''))

	if isinstance(reason, string_types):

		def decorator(func1):

			fmt1 = "Call to deprecated function {name} ({reason})."

			@functools.wraps(func1)
			def new_func1(*args, **kwargs):
				warnings.simplefilter('always', DeprecationWarning)
				warnings.warn(
					fmt1.format(name=func1.__name__, reason=reason),
					category=DeprecationWarning,
					stacklevel=2
				)
				warnings.simplefilter('default', DeprecationWarning)
				return func1(*args, **kwargs)

			return new_func1

		return decorator

	elif inspect.isfunction(reason):

		func2 = reason
		fmt2 = "Call to deprecated function {name}."

		@functools.wraps(func2)
		def new_func2(*args, **kwargs):
			warnings.simplefilter('always', DeprecationWarning)
			warnings.warn(
				fmt2.format(name=func2.__name__),
				category=DeprecationWarning,
				stacklevel=2
			)
			warnings.simplefilter('default', DeprecationWarning)
			return func2(*args, **kwargs)

		return new_func2

@deprecated("Use 'configure_app(docking=True, docking_space=dock_space)'.")
def enable_docking(dock_space: bool = False) -> None:
    internal_dpg.configure_app(docking=True, docking_space=dock_space)

@deprecated("Use 'get_app_configuration()['version']'.")
def get_dearpygui_version() -> str:
    return internal_dpg.get_app_configuration()["version"]

@deprecated("Use 'configure_app(init_file=file)'.")
def set_init_file(file: str = "dpg.ini") -> None:
    internal_dpg.configure_app(init_file=file)

@deprecated("Use 'configure_app(init_file=file, load_init_file=True)'.")
def load_init_file(file: str) -> None:
    internal_dpg.configure_app(init_file=file, load_init_file=True)

@deprecated("Use: `is_viewport_ok(...)`")
def is_viewport_created() -> bool:
    return internal_dpg.is_viewport_ok()

@deprecated("Use: \ncreate_viewport()\nsetup_dearpygui()\nshow_viewport()")
def setup_viewport():
    internal_dpg.create_viewport()
    internal_dpg.setup_dearpygui()
    internal_dpg.show_viewport()

@deprecated("Use: `bind_item_theme(...)`")
def set_item_theme(item : Union[int, str], theme : Union[int, str]) -> None:
	return internal_dpg.bind_item_theme(item, theme)

@deprecated("Use: `bind_item_type_disabled_theme(...)`")
def set_item_type_disabled_theme(item : int, theme : Union[int, str]) -> None:
	return internal_dpg.bind_item_type_disabled_theme(item, theme)

@deprecated("Use: `bind_item_type_theme(...)`")
def set_item_type_theme(item : int, theme : Union[int, str]) -> None:
	return internal_dpg.bind_item_type_theme(item, theme)

@deprecated("Use: `bind_item_font(...)`")
def set_item_font(item : Union[int, str], font : Union[int, str]) -> None:
	return internal_dpg.bind_item_font(item, font)

@deprecated("Use: `add_item_activated_handler(...)`")
def add_activated_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_activated_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_active_handler(...)`")
def add_active_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_active_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_clicked_handler(...)`")
def add_clicked_handler(parent : Union[int, str], button : int =-1, *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_clicked_handler(parent, button, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_deactived_after_edit_handler(...)`")
def add_deactivated_after_edit_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_deactivated_after_edit_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_deactivated_handler(...)`")
def add_deactivated_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_deactivated_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_edited_handler(...)`")
def add_edited_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_edited_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_focus_handler(...)`")
def add_focus_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_focus_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_hover_handler(...)`")
def add_hover_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_hover_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_resize_handler(...)`")
def add_resize_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_resize_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_toggled_open_handler(...)`")
def add_toggled_open_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_toggled_open_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `add_item_visible_handler(...)`")
def add_visible_handler(parent : Union[int, str], *, label: str =None, user_data: Any =None, use_internal_label: bool =True, tag: Union[int, str] =0, callback: Callable =None, show: bool =True) -> Union[int, str]:
	return internal_dpg.add_item_visible_handler(parent, label=label, user_data=user_data, use_internal_label=use_internal_label, tag=tag, callback=callback, show=show)

@deprecated("Use: `bind_colormap(...)`")
def set_colormap(item : Union[int, str], source : Union[int, str]) -> None:
	return internal_dpg.bind_colormap(item, source)

@deprecated("Use: `bind_theme(0)`")
def reset_default_theme(item : Union[int, str], source : Union[int, str]) -> None:
	return internal_dpg.bind_theme(item, source)

@deprecated
def set_staging_mode(mode : bool) -> None:
	pass

@deprecated
def add_table_next_column(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0, parent: Union[int, str] =0, before: Union[int, str] =0, show: bool =True) -> Union[int, str]:
	pass

@deprecated("Use: add_stage")
def add_staging_container(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0) -> Union[int, str]:
	return internal_dpg.add_stage(label=label, user_data=user_data, use_internal_label=use_internal_label, id=id)

@deprecated("Use: stage")
@contextmanager
def staging_container(*, label: str =None, user_data: Any =None, use_internal_label: bool =True, id: Union[int, str] =0) -> Union[int, str]:
	"""
	Undocumented function
	Args:
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
	Yields:
		Union[int, str]
	"""
	try:
		warnings.warn("'staging_container' is deprecated and was changed to 'stage'", DeprecationWarning, 2)
		widget = internal_dpg.add_stage_container(label=label, user_data=user_data, use_internal_label=use_internal_label, id=id)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()