
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

