
		function y = shekel(x)
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
% 
% Shekel function
% Matlab Code by A. Hedar (Nov. 23, 2005).
% The number of variables n = 4
% The parameter m should be adjusted m = 5,7,10.
% The default value of m = 10.
% 
m = 10;
a = ones(10,4);
a(1,:) = 4.0*a(1,:);
a(2,:) = 1.0*a(2,:);
a(3,:) = 8.0*a(3,:);
a(4,:) = 6.0*a(4,:);
for j = 1:2;
   a(5,2*j-1) = 3.0; a(5,2*j) = 7.0; 
   a(6,2*j-1) = 2.0; a(6,2*j) = 9.0; 
   a(7,j)     = 5.0; a(7,j+2) = 3.0;
   a(8,2*j-1) = 8.0; a(8,2*j) = 1.0;
   a(9,2*j-1) = 6.0; a(9,2*j) = 2.0;
   a(10,2*j-1)= 7.0; a(10,2*j)= 3.6;
end
c(1) = 0.1; c(2) = 0.2; c(3) = 0.2; c(4) = 0.4; c(5) = 0.4;
c(6) = 0.6; c(7) = 0.3; c(8) = 0.7; c(9) = 0.5; c(10)= 0.5;
s = 0;
for j = 1:m;
   p = 0;
   for i = 1:4
      p = p+(x(i)-a(j,i))^2;
   end
   s = s+1/(p+c(j));
end
y = -s;
		
		if nargin == 1 || ~noPause
			pause(0.05);
		end
		
	case 0
		% plot function
        shekeldim = 4;
		x = -shekeldim:1:shekeldim;
        x = 1:1000;
		N = length(x);
		F = zeros(N);
		for m=1:N
			for n=1:N
				F(m,n) = foxholes([x(m); x(n)], 1);
			end
		end

		figure;
		contour(x,x,F,10);
        surf(x, x, F);
		title('Contour plot of Shekel''s Foxholes');

	otherwise
		error('Wrong number of input arguments.');
end