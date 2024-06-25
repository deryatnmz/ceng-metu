N=21210;
TotalWeight=zeros(N,1); 
for k=1:N;
    n=250;
    p=0.62;
    U=rand(n,1);
	Y=sum(U<p);
    weight = 0;
    
    s = 0; t = 8; m = 0.22;  % the boundary of the rejection sampling rectangle
    for f=1:Y; 
        CX = 0; CY = m; % initial coordinates for rejection sampling
    	F = 0; %f(0)
    
		while (CY > F); 
            U = rand; V = rand;
            CX = s+(t-s)*U; CY = m*V;
        
            if CX <= 0
                F = 0;
			elseif CX <=2
				F = (0.07)*CX;
			elseif CX <=5
				F = (-0.02)*((CX-4)^2) + 0.22;
			elseif CX <=7
				F = (0.08)*(5-CX)+ 0.2;
			elseif CX <=8
				F= (-0.04)*CX + 0.32;
			else
				F=0;
            end
        end;
        weight = weight + CX;   
    end;
    TotalWeight(k) = weight;
end;
p_est = mean(TotalWeight>640);
expectedWeight = mean(TotalWeight);
stdWeight = std(TotalWeight);
fprintf('Estimated probability = %f\n',p_est);
fprintf('Expected weight = %f\n',expectedWeight);
fprintf('Standard deviation = %f\n',stdWeight);

