function y = foxholes(x, noPause)

switch nargin
	case {1, 2}
        todaysnumber = 5;
        number = 30;
        
                 a = (2 * number).*rand(2, todaysnumber);
                 a = a -number;
               
            c = 1.*rand(1, todaysnumber);

		tmp = 0;
		for i = 1:todaysnumber
			tmp2 = 0;
			for j = 1:2
				tmp2 = tmp2 + (x(j) - a(j,i)).^2;
			end
			tmp = tmp + (1 / (c(i) + tmp2));
        end
        y = tmp * 10;
		
		if nargin == 1 || ~noPause
			pause(0.05);
		end
		
	case 0
        number = 30;
        x = -number:number;
		N = length(x);
		F = zeros(N);
		for m=1:N
			for n=1:N
				F(m,n) = foxholes([x(m); x(n)], 1);
			end
        end
        
        figure('Color','white')

surf(x,x,F);

		title('Contour plot of Shekel''s Foxholes');

	otherwise
		error('Wrong number of input arguments.');
end