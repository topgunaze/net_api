set tabstop=2
set shiftwidth=2
:inoremap main #include <stdio.h> <CR>#include <unistd.h> <CR>#include <stdlib.h> <CR>#include <pthread.h><CR>int <CR>main(int argc, char * argv[]) <CR>{ <CR>	return 0; <CR>}

":inoremap main (c-r)=Main('<tab>') <CR>
function! Main(char)
call append(line(".")+0, "#include <stdio.h>")
endfunction
