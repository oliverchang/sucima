class Ball {
  constructor(spin_angle, spin_strength, trajectory, position, speed) {
    this.spin_angle = spin_angle;
    this.spin_strength = spin_strength;
    this.trajectory = trajectory;
    this.position = position;
    this.speed = speed;
  }

  static decode(buffer) {
    let array = new Uint8Array(buffer);
    return new Ball(array[0] * 2, array[1], array[2], array[3], array[4]);
  }

  static get encodedSize() {
    return 5;
  }

  encode(result) {
    result[0] = ((this.spin_angle + 360) % 360) / 2;
    result[1] = this.spin_strength;
    result[2] = this.trajectory;
    result[3] = this.position;
    result[4] = this.speed;

    return result.buffer;
  }
}

export default Ball;
