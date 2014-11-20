% An implementation of our standard LIF model . Run an example s imulat ion
% like this :
%
%drive = -1:.002:1; %ramp input
%n = 50; %# of neurons
%tauRef = .002; tauRC = .02;
%intercepts = -1+2*rand(1 ,n);
%maxRates = 100+100*rand(1 ,n);
%scales = ((1 ./ (1 - exp ((tauRef - (1 ./ maxRates)) / tauRC))) - 1) ./ (1 - intercepts);
%biases = 1 - scales .* intercepts;
%spikes = referenceLIF(.001, drive, .002, .02, scales, biases);
%imagesc(spikes) , colormap('gray') , xlabel ('Time (ms)') , ylabel('Neuron #')

% dt : time step (s)
% drive : input in the range [1 ,1] (1 X #steps )
% tauRef : spike refractory period
% tauRC: membrane time constant
% scales : list of input scale factors for different
% neurons (1 X #neurons )
% biases : list of additive input biases for different neurons (1 X #neurons )

function spikes = referenceLIF(dt, drive, tauRef,tauRC, scales, biases)
V = zeros(size(scales));
lastSpike = -1*ones(size(scales));
spikes = zeros (length(scales) , length(drive));

    for i = 1:length( drive )
        endTime = dt * i ;
        intTimes = min(dt , max(0 , endTime - lastSpike - tauRef));

        current = biases + scales .* drive(i)';
        dV = (current - V) ./ tauRC;

        V = V + intTimes .* dV;
        V = max(0, V);

        spikeIndices = find(V >= 1);

        % here we are interpolating the spike time between time steps (important at high
        % rates)
        lastSpike(spikeIndices) = endTime - (V(spikeIndices)-1) ./ dV(spikeIndices);

        V(spikeIndices) = 0;
        spikes(spikeIndices, i) = 1;
    end
end
