extern module
global asics
call asics.share
in al, module.byte
out al, module.exbyte
in al, module.exbyte
out al, module.byte