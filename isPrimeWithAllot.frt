: isPrimeWithAllot
        0 swap 
	dup 2 /  1 do
                dup r@ %
                if else swap 1 + swap            
                then 
        loop
swap
1 - 
swap drop
if 
	0  
	else 1
then 
4 allot dup dup !
;

