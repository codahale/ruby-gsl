require 'rake/gempackagetask'

require "lib/gsl/version"

PKG_NAME = "ruby-gsl"
PKG_VERSION   = GSL::VERSION::STRING
PKG_FILE_NAME = "#{PKG_NAME}-#{PKG_VERSION}"
PKG_FILES = FileList[
  '[A-Z]*',
  'lib/**/*.rb', 
  'spec/**/*.rb', 
  'ext/*.c',
  'ext/*.h',
  'ext/*.rb'
]

spec = Gem::Specification.new do |s|
  s.name = PKG_NAME
  s.version = PKG_VERSION
  s.summary = "Ruby bindings for the GNU Scientific Library to the Ruby."
  s.description = <<-EOF
    TODO
  EOF

  s.files = PKG_FILES.to_a
  s.require_path = 'lib'

  s.has_rdoc = true
  s.rdoc_options = @rdoc_rake_task.options
  s.extra_rdoc_files = @rdoc_rake_task.rdoc_files.to_a
  
  s.extensions = FileList["ext/extconf.rb"].to_a
  
  s.autorequire = 'gsl'
  s.authors = ["Coda Hale"]
  s.email = "coda.hale@gmail.com"
end

Rake::GemPackageTask.new(spec) do |pkg|
  # pkg.need_zip = true
  # pkg.need_tar = true
end

desc 'Install the package as a gem.'
task :install => [:clean, :package] do
  sh "sudo gem install pkg/*.gem --no-rdoc --no-ri"
end