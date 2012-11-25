function y = foxholes(x, noPause)
%FOXHOLES  Evaluate Shekel's Foxholes function.
%		Y = FOXHOLES(X) with a two-element input vector X evaluates Shekel's
%		Foxholes function at X. The elements of X have to be from the interval
%		[-65.536, 65.536]. The global minimum of this function is about
%		0.998004 at X = [-32, -32].
%
%		FOXHOLES (without input arguments) displays the function as a contour
%		plot.
%
%		Markus Buehren
%		Last modified 03.02.2008

switch nargin
    case {1, 2}
        % compute function value
        
        sum = 0.0;
        
        %there appears to be no effect
        %holessqrt = 5;
        
        for i = 1:30
            sp = 0.0;
            for j = 1:x(1)
                h = x(2) - random('Normal',5,2);
                %h = x(2) - a(i, j);
                sp = sum + h * h;
            end
            sum = sum - 1.0 / (sp + random('Normal',.5,.2));
        end
        
        y = -sum;
        
        if nargin == 1 || ~noPause
            pause(0.05);
        end
        
    case 0
        % plot function
        x = 1:10;
        N = length(x);
        F = zeros(N);
        
        for m = 1:N
            for n = 1:N
                F(m, n) = foxholes([x(m); x(n)], 1);
            end
        end
        
        figure;
        
        contour3(x,x,F,10);
        title('Contour plot of Shekel''s Foxholes');
        %new stuff...
        surf(x,x,F);
        shading interp
        %new stuff/
    otherwise
        error('Wrong number of input arguments.');
end