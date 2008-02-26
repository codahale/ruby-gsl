require 'rake/clean'

CLEAN.include(
  "ext/Makefile",
  "ext/*.o",
  "ext/*.bundle",
  "ext/*.so"  
)

desc "Clean, then compile the extension."
task :recompile => [:clean] do
  Dir.chdir('./ext')
  system "ruby extconf.rb && make"
  Dir.chdir("..")
end

desc "Compile the extension."
task :compile do
  Dir.chdir('./ext')
  system "make"
  Dir.chdir("..")
end