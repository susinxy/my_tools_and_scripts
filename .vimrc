set tabstop=4
set shiftwidth=4
set expandtab

function! GenCstag(...)
"    silent execute '!' . 'cscope -b ' . a:000
"    silent execute '!' . 'ctags - R ' . a:000
    echo a:000
endfunction
