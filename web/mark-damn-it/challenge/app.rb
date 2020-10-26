require 'erb'
require 'fileutils'
require 'kramdown'
require 'securerandom'
require 'sinatra'
require 'timeout'

def markdown_to_html(md)
  Kramdown::Document.new(md).to_html
end

get '/' do
  input_md = File.open('example.md').read(4096)
  begin
    output = markdown_to_html(input_md) # to pass index.html.erb
    content_type 'text/html'
    ERB.new(File.read('templates/index.html.erb')).result(binding)
  rescue
    redirect '/', 301
  end
end

post '/' do
  input_md = params[:md].to_s.force_encoding('UTF-8')
  begin
    Timeout.timeout(5) do
      output = markdown_to_html(input_md) # to pass index.html.erb
      content_type 'text/html'
      ERB.new(File.read('templates/index.html.erb')).result(binding)
    end
  rescue
    redirect '/', 301
  end
end
