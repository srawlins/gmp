require '../ext/gmp'
require 'benchmark'

def multiply_run(multiplicands)
  random_state = GMP::RandState.new

  if multiplicands.class == Array
    m, n = multiplicands[0], multiplicands[1]
    x = random_state.urandomb(m)
    y = random_state.urandomb(n)
  else
    m = multiplicands
    x = random_state.urandomb(m)
    y = x
  end
  
  calibrate_iterations = 1
  diff = Benchmark.measure { z = x * y }.total
  while diff < 0.001
    calibrate_iterations *= 4
    diff = Benchmark.measure { calibrate_iterations.times {z = x * y} }.total
    puts "diff: %6.4f, calibrate_iterations: #{calibrate_iterations}" % diff
  end
  diff = diff / calibrate_iterations
  diff = 0.01 if diff < 0.01
  iterations = (1 + (1e4 / diff)).to_i
  
  if multiplicands.class == Array
    print "Multiplying %i-bit number with %i-bit number %i times..." % [m, n, iterations]
  else
    print "Squaring a %i-bit number %i times..." % [m, iterations]
  end
  STDOUT.flush
  
  results = Benchmark.measure {
    iterations.times do
      z = x * y
    end
  }
  
  puts "done!"
  ops_per_sec = iterations.to_f / (results.total)
  puts "RESULT: %f operations per second" % ops_per_sec
end