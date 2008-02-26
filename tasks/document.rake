require 'rake/rdoctask'

desc 'Generate RDoc'
@rdoc_rake_task = Rake::RDocTask.new do |rdoc|
  rdoc.rdoc_dir = 'doc/rdoc'
  rdoc.options << '--title' << 'bcrypt-ruby' << '--line-numbers' << '--inline-source' << '--main' << 'README'
  rdoc.template = ENV['TEMPLATE'] if ENV['TEMPLATE']
  rdoc.rdoc_files.include('README', 'COPYING', 'CHANGELOG', 'lib/**/*.rb', "ext/**/*.c")
end