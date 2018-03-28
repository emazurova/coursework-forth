: isPrime
	dup 0 < if
		drop 0
	else 
		dup 4 < if drop 1
			else
			0 swap 
			dup 2 / 2 do
				dup r@ %
				if else swap 1 + swap 
				then 
			loop
			drop
			0 = if 1 else 0
			then 
	then ;


: rad	
dup 0 = if
				drop 0
			else
				1 swap dup 2 / 1 + 2 do	
				dup r@ % if else
						r@ isPrime if
						swap r@ * swap
						else then
					then
				loop drop
				then ;									
