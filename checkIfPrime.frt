: isPrime
        0 swap 
	dup 2 /  1 do
                dup r@ %
                if else swap 1 + swap            
                then 
        loop
swap
1 - 
if 
	." 0" cr 
	else ." 1" cr 
then ;

