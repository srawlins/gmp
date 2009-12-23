# Benchmarks based on gmpbench-0.2

multiply_args = [128, 512, 8192, 131072, 2097152, [128,128], [512,512],
                 [8192,8192], [131072,131072], [2097152,2097152],
                 [15000,10000], [20000,10000], [30000,10000], [16777216,512],
                 [16777216,262144]
                ]
multiply_weight = 1
require 'multiply'
multiply_args.each do |multiplicands|
  multiply_run(multiplicands)
end