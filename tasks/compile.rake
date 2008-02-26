require 'rake/clean'

CLEAN.include(
  "ext/Makefile",
  "ext/*.o",
  "ext/*.bundle",
  "ext/*.so"  
)

desc "Clean, then compile the extension."
task :compile => [:clean] do
  Dir.chdir('./ext')
  system "ruby extconf.rb && make"
  Dir.chdir("..")
end