require 'net/http'
require 'uri'
require 'json'
require 'fileutils'

class CloudflareFileStorage
  def initialize(worker_url)
    @worker_url = worker_url.chomp('/')
  end

  def upload_file(file_path)
    raise "File not found: #{file_path}" unless File.exist?(file_path)

    uri = URI("#{@worker_url}/upload")
    file = File.open(file_path, 'rb')
    req = Net::HTTP::Post.new(uri)
    req.set_form([['file', File.open(file_path)]], 'multipart/form-data')

    res = Net::HTTP.start(uri.hostname, uri.port) { |http| http.request(req) }
    file.close
    res.body
  end

  def download_file(file_name, save_path)
    uri = URI("#{@worker_url}/download/#{file_name}")
    res = Net::HTTP.get_response(uri)
    raise "Error downloading file: #{res.message}" unless res.is_a?(Net::HTTPSuccess)

    file_path = File.join(save_path, file_name)
    File.write(file_path, res.body)
    res.body
  end

  def delete_file(file_name)
    uri = URI("#{@worker_url}/delete/#{file_name}")
    req = Net::HTTP::Delete.new(uri)
    res = Net::HTTP.start(uri.hostname, uri.port) { |http| http.request(req) }
    res.body
  end
end
