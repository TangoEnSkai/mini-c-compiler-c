void main ( )
{ 
	int i = 100 ;
	int sum = 0 ;		
	int x = 0 ;
	int y = 0 ;
	int z = 0 ;
	while ( i <= 999 ) {
		x = i / 100 ;
		y = ( i / 10 ) % 10 ;
		z = ( i % 10 ) ;
		sum = ( x * x * x ) + ( y * y * y ) + ( z * z * z ) ;
		if ( i == sum ) write ( i ) ;
		++i ;
	}
}
