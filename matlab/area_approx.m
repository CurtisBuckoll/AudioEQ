resolution = 0.001;
e = 2.718281828459045235;

f_actual = @(x)2.*(1/2.*sqrt(2*x - x.^2).*(x - 1) - 1/2*asin(1 - x) + pi/4);
f_sigmoid = @(x,c)pi./(1+e.^(-c*(x-1)));

domain = 0:resolution:2;

sq_diff = (actual(domain) - sigmoid(domain)).^2
sum(sq_diff)*resolution

hold on
plot(domain, actual(domain))

plot(domain, sigmoid(domain))
hold off

function y = actual(x)
    y = 2.*(1/2.*sqrt(2*x - x.^2).*(x - 1) - 1/2*asin(1 - x) + pi/4);
end

function y = sigmoid(x)
    e = 2.718281828459045235;
    y = pi./(1+e.^(-(pi)*(x-1)));
end