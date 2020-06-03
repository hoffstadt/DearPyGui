import sys
sys.argv = ['']
class CatchOutErr:
    def __init__(self):
        self.value = ''
    def write(self, txt):
        self.value += txt
    def flush(self):
        self.value = ''

catchOutErr = CatchOutErr()
sys.stdout = catchOutErr
sys.stderr = catchOutErr